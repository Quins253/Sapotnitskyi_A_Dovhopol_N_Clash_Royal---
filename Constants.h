#pragma once;
const int tilesX = 18;
const int tilesY = 32;
const int baseTileSize = 25;
const int baseMenuHeight = 100;
const int baseManaBarHeight = 30;

const int baseFieldHeight = tilesY * baseTileSize;
const int baseWindowHeight = baseFieldHeight + baseManaBarHeight + baseMenuHeight;

const float baseNextCardXOffset = 43; 
const float baseNextCardYOffset = 0;  

const int targetHeight = 1080;
const float scalingFactor = static_cast<float>(targetHeight) / baseWindowHeight;

const float tileSize = baseTileSize * scalingFactor;
const float menuHeight = baseMenuHeight * scalingFactor;
const float manaBarHeight = baseManaBarHeight * scalingFactor;
const float fieldHeight = tilesY * tileSize;
const float windowHeight = fieldHeight + manaBarHeight + menuHeight;
const float windowWidth = tilesX * tileSize;


const float speedScale = 60.f;