# OpenGL Camera Movement + Textured Cube Demo

A simple OpenGL project based on [LearnOpenGL](https://learnopengl.com/) that demonstrates rendering multiple textured cubes in 3D space, with WASD-based scaling and mix value control, and arrow-key based camera movement.

![image](https://github.com/user-attachments/assets/b50bfd6a-02fc-453d-9b0e-4fe135d4bd62)

---

## 🎮 Features

- ✅ Basic cube rendering (36 vertices)
- 🎥 Dynamic camera movement (Arrow keys)
- 🧱 Texture blending with two images
- ⚙️ Runtime adjustable mix & scale (WASD)
- 🎨 Time-based color and rotation animations
- 🌌 Perspective projection & view matrix control

---

## 🛠 Setup

If you're following [LearnOpenGL.com](https://learnopengl.com/), the setup should be pretty self-explanatory. Make sure:

1. You have all required wrappers/libraries:
   - `GLFW`
   - `GLAD`
   - `GLM`
   - `stb_image.h`
2. You have valid shader files (`3.3.shader.vs` and `3.3.shader.fs`) in your working directory.
3. Update image paths in the code if needed.

```cpp
stbi_load("path/to/illumanti_texture.jpg", &width, &height, &nrChannels, 0);
stbi_load("path/to/spiderman.png", &width, &height, &nrChannels, 0);
