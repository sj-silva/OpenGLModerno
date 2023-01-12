#### 🎯 Element Buffer Object - (EBO)
```cpp

        glGenBuffers(1, &QuadEBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        . 
        .
        .
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

#### Link para o vídeo no YouTube:

[![Link to video 13](https://img.youtube.com/vi/ZmqbZ8xR4Fg/default.jpg)](https://youtu.be/ZmqbZ8xR4Fg)



