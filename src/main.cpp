#include "Intersectable.hpp"
#include "Light.hpp"
#include "MaterialLoader.hpp"
#include "SceneLoader.hpp"
#include "Util/ReinhardToneMapping.hpp"
#include "Util/Window.hpp"

#include <boost/program_options.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <random>

glm::mat4 generatePixelstoNDC(unsigned width, unsigned height) {
    return glm::ortho(-0.5f, width - 0.5f, height - 0.5f, -0.5f);
}

namespace {
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution dis(-0.5f, 0.5f);
}  // namespace

Ray generateRay(unsigned x, unsigned y, unsigned num_bounces, glm::mat4 unproj, glm::vec3 origin) {
    glm::vec4 world = unproj * glm::vec4{x + dis(gen), y + dis(gen), 0.0f, 1.0f};
    glm::vec3 direction = glm::normalize(glm::vec3{world / world.w} - origin);
    return Ray(origin, direction, num_bounces);
}

std::vector<Ray> generateScreenRays(unsigned width, unsigned height, glm::mat4 unproj, glm::vec3 origin) {
    std::vector<Ray> rays;
    rays.reserve(width * height);
    unproj = unproj * generatePixelstoNDC(width, height);
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            rays.push_back(generateRay(x, y, 3, unproj, origin));
        }
    }
    return rays;
}

template <ExecutionPolicy P>
void accumulateColors(P policy, const Scene& scene, unsigned width, unsigned height, unsigned num_samples, glm::mat4 proj_view_inv, std::vector<glm::vec3>& accumulated_colors) {
    auto rays = generateScreenRays(width, height, proj_view_inv, scene.camera.position);
    std::transform(
        policy, rays.begin(), rays.end(), accumulated_colors.begin(), accumulated_colors.begin(),
        [&](Ray r, glm::vec3 color) {
            for (unsigned i = 0; i < num_samples; i++) {
                color += sceneIntersectColor(scene, r);
            }
            return color;
        });
}

glm::mat4 getProjViewInv(const Window& window, const Scene& scene) {
    glm::mat4 proj = windowPerspective(window, scene.eps);
    glm::mat4 view = cameraView(scene.camera);
    return glm::inverse(proj * view);
}

template <ExecutionPolicy P>
auto runBenchmark(P policy, const Window& window, const Scene& scene) {
    auto [width, height] = window.dimensions();
    glm::mat4 proj_view_inv = getProjViewInv(window, scene);
    std::vector<glm::vec3> colors{width * height};

    auto start = std::chrono::steady_clock::now();
    accumulateColors(policy, scene, width, height, window.samples, proj_view_inv, colors);
    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

template <ExecutionPolicy P>
void renderFrame(P policy, Window& window, const Scene& scene, std::vector<glm::vec3>& accumulated_colors, unsigned& accumulated_samples) {
    auto [width, height] = window.dimensions();
    glm::mat4 proj_view_inv = getProjViewInv(window, scene);

    accumulateColors(policy, scene, width, height, window.samples, proj_view_inv, accumulated_colors);
    accumulated_samples += window.samples;
    auto pixels = std::views::transform(
        accumulated_colors,
        [&](glm::vec3 pixel) { return pixel / float(accumulated_samples); });
    auto tone_mapped_pixels = reinhardToneMapPixels(policy, pixels.begin(), pixels.end());
    window.setPixels(tone_mapped_pixels.begin(), tone_mapped_pixels.end());
    window.update();
}

template <ExecutionPolicy P>
void runRender(P policy, Window& window, const Scene& scene) {
    auto [width, height] = window.dimensions();
    std::vector<glm::vec3> accumulated_colors{width * height, glm::vec3{0.0f}};
    unsigned accumulated_samples = 0;

    while (!window.shouldClose()) {
        renderFrame(policy, window, scene, accumulated_colors, accumulated_samples);
    }
}

int main(int argc, const char* argv[]) {
    namespace bpo = boost::program_options;

    unsigned width = 1280;
    unsigned height = 720;
    unsigned pixel_samples = 16;
    std::string scene_name = "Default";

    bpo::options_description options;
    // clang-format off
    options.add_options()
        ("help", "show this message")
        ("benchmark", "render a single frame and measure the time it takes")
        ("width,w", bpo::value(&width), "set window width")
        ("height,h", bpo::value(&height), "set window height")
        ("samples,s", bpo::value(&pixel_samples), "the number of samples per pixel")
    ;
    // clang-format on

    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(argc, argv, options), vm);
    bpo::notify(vm);

    if (vm.contains("help")) {
        std::cout << "Usage: " << argv[0] << " [options]:\n";
        std::cout << options << "\n";
        return 0;
    }

    bool benchmark = vm.contains("benchmark");

    NamedScene scene = loadScene("");

    Window window(width, height);
    window.samples = pixel_samples;
    window.hfov = 45.0f;
    window.setTitle(scene.name);

    auto policy = std::execution::par_unseq;

    if (benchmark) {
        window.hide();
        auto render_time = runBenchmark(policy, window, scene.scene);
        std::cout << "Rendered scene \"" << scene.name << "\" "
                  << "(" << width << "x" << height << ", " << pixel_samples << " samples) "
                  << "in " << render_time << " milliseconds\n";
    } else {
        runRender(policy, window, scene.scene);
    }
}
