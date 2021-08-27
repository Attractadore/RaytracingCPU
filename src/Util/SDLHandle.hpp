#pragma once

class SDLHandle {
public:
    SDLHandle();
    ~SDLHandle() noexcept;
    SDLHandle(const SDLHandle& other) = default;
    SDLHandle(SDLHandle&& other) noexcept = default;
    SDLHandle& operator=(const SDLHandle& other) = default;
    SDLHandle& operator=(SDLHandle&& other) noexcept = default;
};
