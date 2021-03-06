#include "rtr/shaders/TerrainShaders.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

TerrainShaders::TerrainShaders(int textureResolution, std::vector<std::string> texture_path, vec4 sunDirection, float amplitude, float frequency, int terrainResolution, int tileNumber, int subdivide) :
	mTexturePath(texture_path),
	mSunDirection(sunDirection),
	mAmplitude(amplitude),
	mFrequency(frequency),
	mTerrainResolution(terrainResolution),
	mTileNumber(tileNumber),
	mSubdivide(subdivide)
{
		mTextureID0 = generateTexture(textureResolution, mTexturePath[0].c_str());
		mTextureID1 = generateTexture(textureResolution, mTexturePath[1].c_str());
		mTextureID2 = generateTexture(textureResolution, mTexturePath[2].c_str());

}

void TerrainShaders::activate()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mTextureID0);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, mTextureID1);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, mTextureID2);

	SimpleShaders::activate();
}

// Find uniform variables in the shaders and store their location
void TerrainShaders::locateUniforms()
{
	glUseProgram(mShaderProgram);


	mModelLocation = glGetUniformLocation(mShaderProgram, "model");
	if (mModelLocation == -1){
		printf("[TerrainShaders] Model location not found\n");
	}

	mModelInvTLocation = glGetUniformLocation(mShaderProgram, "modelInvT");
	if (mModelInvTLocation == -1)
		printf("[TerrainShaders] ModelInvT location not found\n");

	mViewLocation = glGetUniformLocation(mShaderProgram, "view");
	if (mViewLocation == -1)
		printf("[TerrainShaders] View location not found\n");

	
	mProjectionLocation = glGetUniformLocation(mShaderProgram, "projection");
	if (mProjectionLocation == -1)
		printf("[TerrainShaders] Projection location not found\n");

	mWorldSunDirectionLocation = glGetUniformLocation(mShaderProgram, "worldSunDirection");
	if (mWorldSunDirectionLocation == -1)
		printf("[TerrainShaders] WorldSunDirection location not found\n");

	glUniform4fv(mWorldSunDirectionLocation, 1, &mSunDirection[0]);

	mTextureSamplerLocation0 = glGetUniformLocation(mShaderProgram, "terrain0");
	if (mTextureSamplerLocation0 == -1)
		printf("[TerrainShaders] Texture sampler 0 not found\n");
	glUniform1i(mTextureSamplerLocation0, 0);

	mTextureSamplerLocation1 = glGetUniformLocation(mShaderProgram, "terrain1");
	if (mTextureSamplerLocation1 == -1)
		printf("[TerrainShaders] Texture sampler 1 not found\n");
	glUniform1i(mTextureSamplerLocation1, 1);

	mTextureSamplerLocation2 = glGetUniformLocation(mShaderProgram, "terrain2");
	if (mTextureSamplerLocation2 == -1)
		printf("[TerrainShaders] Texture sampler 2 not found\n");
	glUniform1i(mTextureSamplerLocation2, 2);

	mAmplitudeLocation = glGetUniformLocation(mShaderProgram, "amplitude");
	if (mAmplitudeLocation == -1)
		printf("[TerrainShaders] Amplitude not found\n");
	
	glUniform1f(mAmplitudeLocation, mAmplitude);

	mFrequencyLocation = glGetUniformLocation(mShaderProgram, "frequency");
	if (mFrequencyLocation == -1)
		printf("[TerrainShaders] Frequency not found\n");

	glUniform1f(mFrequencyLocation, mFrequency);

	mTimeLocation = glGetUniformLocation(mShaderProgram, "timeTranslate");
	if (mTimeLocation == -1)
		printf("[TerrainShaders] Time not found\n");

	mTerrainResLocation = glGetUniformLocation(mShaderProgram, "terrainResolution");
	if (mTimeLocation == -1)
		printf("[TerrainShaders] TerrainResolution not found\n");
	
	glUniform1i(mTerrainResLocation, mTerrainResolution);

	mTileNoLocation = glGetUniformLocation(mShaderProgram, "textureTileNumber");
	if (mTileNoLocation == -1)
		printf("[TerrainShaders] Tilenumber not found\n");

	glUniform1i(mTileNoLocation, mTileNumber);

	mSubdivideLocation = glGetUniformLocation(mShaderProgram, "subdivide");
	if (mSubdivideLocation == -1)
		printf("[TerrainShaders] Subdivide not found\n");

	glUniform1i(mSubdivideLocation, mSubdivide);

	mTimeDayLocation = glGetUniformLocation(mShaderProgram, "time");
	if (mTimeDayLocation == -1)
		printf("[TerrainShader] time location not found\n");

	mRotationLocation = glGetUniformLocation(mShaderProgram, "sunRotation");
	if (mRotationLocation == -1)
		printf("[TerrainShader] Sun rotation location not found\n");
}


void TerrainShaders::setModelMatrix(const mat4& transformMatrix)
{
	if (mModelLocation < 0)
		printf("[TerrainShaders] uniform location for 'model' not known\n");

	glUseProgram(mShaderProgram);												
	glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, &transformMatrix[0][0]);	

	if (mModelInvTLocation < 0)
		printf("[TerrainShaders] uniform location for 'modelInvT' not known\n");

	mat3 normalMatrix = mat3(transformMatrix); 
	normalMatrix = glm::transpose(glm::inverse(normalMatrix));
	glUniformMatrix3fv(mModelInvTLocation, 1, GL_FALSE, &normalMatrix[0][0]);
}


// View Matrix
void TerrainShaders::setViewMatrix(const glm::mat4& viewMatrix)
{
	if (mViewLocation < 0)
		printf("[TerrainShaders] uniform location for 'view' not known\n");

	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, &viewMatrix[0][0]);

}

// Projection Matrix
void TerrainShaders::setProjectionMatrix(const glm::mat4& projMatrix)
{
	if (mProjectionLocation < 0)
		printf("[TerrainShaders] uniform location for 'projection' not known\n");

	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &projMatrix[0][0]);
}

// Amplitude
void TerrainShaders::setAmplitude(const float amplitude)
{
	if(mAmplitudeLocation < 0)
		printf("[TerrainShaders] uniform location for 'amplitude' not known\n");

	glUseProgram(mShaderProgram);
	glUniform1f(mAmplitudeLocation, amplitude);
}

// Frequency
void TerrainShaders::setFrequency(const float frequency)
{
	if (mFrequencyLocation < 0)
		printf("[TerrainShaders] uniform location for 'frequency' not known\n");

	glUseProgram(mShaderProgram);
	glUniform1f(mFrequencyLocation, frequency);
}

// Vector for translating terrain for fractal input
void TerrainShaders::setTime(const glm::vec3& timeTranslate)
{
	if (mTimeLocation < 0)
		printf("[TerrainShaders] uniform location for 'time' not known\n");
	
	glUseProgram(mShaderProgram);
	glUniform3fv(mTimeLocation, 1, &timeTranslate[0]);
}

void TerrainShaders::setDayTime(float time)
{
	if(mTimeDayLocation < 0)
		printf("[TerrainShaders] uniform location for 'timeDay' not known\n");
	int timeInt = static_cast<int>(time) % 24000;
	glUseProgram(mShaderProgram);
	glUniform1f(mTimeDayLocation, static_cast<float> (timeInt));
}

void TerrainShaders::setSunRotation(const mat4& rotationMatrix)
{
	if (mRotationLocation == -1)
		printf("[TerrainShaders] uniform location for 'sun rotation' not known\n");

	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(mRotationLocation, 1, GL_FALSE, &rotationMatrix[0][0]);
}
// Load texture set all parameters
GLuint TerrainShaders::generateTexture(int imageResolution, const char* path)
{
	GLuint mId = 0;
	unsigned char* texture_data = SOIL_load_image(path, &imageResolution, &imageResolution, 0, SOIL_LOAD_RGBA);
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageResolution, imageResolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return mId;
}
