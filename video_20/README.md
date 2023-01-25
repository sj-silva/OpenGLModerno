# Classe Cubo

```cpp
class Cube
{
public:
	Cube();
	~Cube();

	void Create();

	void SetPosition(float x, float y, float z);
	
	glm::mat4 GetPosition() const;
	
	void Draw();
	
private:
	unsigned int _VAO;
	unsigned int _EBO;
	unsigned int _positionVBO;
	unsigned int _colorVBO;

	glm::vec3 _position;
	glm::mat4 _model;

	void CreateBuffers();
	void FillBuffers();
	void LinkBuffers();

	void UpdatePosition();
};


```
