#include <cmath>
#include <memory>
#include "textureStorage.h"
#include "textures.h"

void convertColorToSrgb(sf::Color& color) // Necessary for not having too depressingly dark colors
{
	color.r = sqrt(int(color.r) * 256);
	color.g = sqrt(int(color.g) * 256);
	color.b = sqrt(int(color.b) * 256);
}



void texStorage::Texture::setGlTexCoord2f(float x, float y)
{
	auto coord = this->convertTexCoord(x, y);
	glTexCoord2f(coord.x, coord.y);
}

texStorage::TextureAtlas::TextureAtlas()
{
    std::vector<std::string> filepaths;
    for (int i = 0; i < int(textures::FaceTexture::enumSize); ++i)
        filepaths.push_back("");
    
    filepaths[int(textures::FaceTexture::stone)] = "blocks/stone.png";
    filepaths[int(textures::FaceTexture::dirt)] = "blocks/dirt.png";
    filepaths[int(textures::FaceTexture::grass_side)] = "blocks/grass_side.png";
    filepaths[int(textures::FaceTexture::grass_top)] = "blocks/grass_top.png";
    filepaths[int(textures::FaceTexture::wewd_side)] = "blocks/wewd_side.png";
    filepaths[int(textures::FaceTexture::wewd_top)] = "blocks/wewd_top.png";
    filepaths[int(textures::FaceTexture::leaves)] = "blocks/leaves.png";


    sf::Vector2u atlasSize(0, 0);
    std::vector<std::shared_ptr<sf::Image>> images;

    // Loading the images
    for (auto path : filepaths)
    {
        auto image = std::make_shared<sf::Image>();
        image->loadFromFile("resources/textures/" + path);
        images.push_back(image);
    }

    // Calculating the atlas size

    int width = 0, height = 0;

    std::vector<sf::Vector2u> texelTexCoords;
    std::vector<sf::Vector2u> imageSizes;

    for (auto image : images)
    {
        texelTexCoords.push_back(sf::Vector2u(width, 0));

        sf::Vector2u imageSize = image->getSize();
        imageSizes.push_back(imageSize);
        if (imageSize.y > height)
            height = imageSize.y;


        width += imageSize.x;
    }

    // loading the images

    sf::Image finalAtlas;
    finalAtlas.create(width, height);

    for (int i = 0; i < images.size(); ++i)
    {
        finalAtlas.copy(*images[i], texelTexCoords[i].x, texelTexCoords[i].y);
    }

    // creating the textures

    for (int i = 0; i < images.size(); ++i)
    {
        this->textures.push_back(
            {
                sf::Vector2f(float(texelTexCoords[i].x) / float(width), float(texelTexCoords[i].y) / float(height)),
                sf::Vector2f(float(texelTexCoords[i].x + imageSizes[i].x) / float(width), float(texelTexCoords[i].y + imageSizes[i].y) / float(height)),
                *this
            }
        );
    }

    // uploading the final texture

    this->finalTexture.loadFromImage(finalAtlas);
    this->finalTexture.setSmooth(false);
}

void texStorage::Texture::operator=(const Texture& other)
{
    new(this) Texture(other);
}

void texStorage::TextureAtlas::bind()
{
    sf::Texture::bind(&this->finalTexture);
}

sf::Vector2f texStorage::Texture::convertTexCoord(float x, float y)
{
    return
    {
        this->begin.x + (this->end.x - this->begin.x) * x,
        this->begin.y + (this->end.y - this->begin.y) * y
    };
}
