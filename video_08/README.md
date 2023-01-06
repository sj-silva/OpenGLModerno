#### 🎯Objetivos:
- Uniforms
  - glGetUniformLocation 
  - glUniform3fv

```cpp
    glm::vec3 myColor(1.f, 0.f, 1.f);

    int colorId = glGetUniformLocation(programId, "triangleColor");

    glUniform3fv(colorId, 1, &myColor.x);
```


#### Link para o vídeo no YouTube:

[![Link to video 08](https://img.youtube.com/vi/lz5c1WpBiMg/default.jpg)](https://youtu.be/lz5c1WpBiMg)


<p align="center">
  <img width="320" height="240" src="modern01.png">
</p>


