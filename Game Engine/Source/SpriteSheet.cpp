#include "SpriteSheet.h"

const int SpriteSheet::TYPE_INVALID = -1;
const int SpriteSheet::TYPE_ARBITRARY_OFFSETS = 0;
const int SpriteSheet::TYPE_SINGLE_GRID = 1;
const int SpriteSheet::TYPE_MULTIPLE_GRIDS = 2;

SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet,
						 SpriteSheetOffset * offsets,
						 int numberOfSprites)
							: name(NULL),
							  spriteSheet(externalSpriteSheet) {
	for(int i=0;i<numberOfSprites;i++) {
		sprites.push_back(new Sprite(offsets[i].x,
									 offsets[i].y,
									 offsets[i].w,
									 offsets[i].h,
									 spriteSheet->getTexture(),
									 spriteSheet->getSprite()));
	}
}

SpriteSheet::SpriteSheet(Sprite * externalSpriteSheet,
						 int xOffset,
						 int yOffset,
						 int width,
						 int height,
						 int xIncrement,
						 int yIncrement,
						 bool horizontal,
						 int numberOfRows,
						 int numberOfColumns)
							: name(NULL),
							  spriteSheet(externalSpriteSheet) {
	int xPos = xOffset;
	int yPos = yOffset;
	for(int i=0;i<numberOfRows;i++) {
		for(int j=0;j<numberOfColumns;j++) {
			sprites.push_back(new Sprite(xPos,
										 yPos,
										 width,
										 height,
										 spriteSheet->getTexture(),
										 spriteSheet->getSprite()));
			if(horizontal) { xPos += xIncrement; }
			else { yPos += yIncrement; }
		}
		if(horizontal) { yPos += yIncrement; xPos = xOffset; }
		else { xPos += xIncrement; yPos = yOffset; }
	}
}

SpriteSheet::~SpriteSheet() {
	for(unsigned int i=0;i<sprites.size();i++) {
		if(sprites.at(i) != NULL) {
			delete sprites.at(i);
		}
	}
	delete spriteSheet;
	if(name != NULL) { delete [] name; }
}

int SpriteSheet::size() { return sprites.size(); }

Sprite * SpriteSheet::elementAt(int index) {
	return this->getSprite(index);
}

Sprite * SpriteSheet::getSprite(int index) {
	if(index < 0 || index >= (int) sprites.size()) { return NULL; }
	return this->sprites.at(index);
}

Sprite * SpriteSheet::getSprite(char * name) {
	if(name == NULL || strlen(name) == 0) { return NULL; }
	for(unsigned int i=0;i<sprites.size();i++) {
		if(sprites.at(i)->getName() != NULL && _stricmp(sprites.at(i)->getName(), name) == 0) {
			return sprites.at(i);
		}
	}
	return NULL;
}

vector<Sprite *> * SpriteSheet::getSprites(int startIndex, int endIndex) {
	if(startIndex < 0 || endIndex >= (int) sprites.size()) { return NULL; }
	vector<Sprite *> * spriteGroup = new vector<Sprite *>;
	for(int i=startIndex;i<=endIndex;i++) {
		spriteGroup->push_back(sprites.at(i));
	}
	return spriteGroup;
}

char * SpriteSheet::getName() { return name; }

void SpriteSheet::setName(char * name) {
	if(name == NULL) { return; }

	if(this->name != NULL) {
		delete [] this->name;
	}
	this->name = new char[strlen(name) + 1];
	strcpy_s(this->name, strlen(name) + 1, name);
}

int SpriteSheet::parseType(const char * data) {
	int type = TYPE_INVALID;
	if(data == NULL) { return type; }
	char * typeString = strtrimcpy(data);
	
	if(_stricmp(typeString, "Arbitrary Offsets") == 0) {
		type = TYPE_ARBITRARY_OFFSETS;
	}
	else if(_stricmp(typeString, "Single Grid") == 0) {
		type = TYPE_SINGLE_GRID;
	}
	else if(_stricmp(typeString, "Multiple Grids") == 0) {
		type = TYPE_MULTIPLE_GRIDS;
	}
	delete [] typeString;
	return type;
}

SpriteSheet * SpriteSheet::parseFrom(ifstream & in, char * spriteDirectory, LPDIRECT3DDEVICE9 d3dDevice) {
	if(in.bad() || spriteDirectory == NULL) {
		printf("Cannot parse sprite sheet from invalid parameter(s).");
		return NULL;
	}

	char input[256];
	char * data;

	Variables properties;
	char * spriteSheetName;
	int spriteSheetType;
	char * spriteSheetFileName;
	Sprite * spriteSheetImage;
	SpriteSheet * spriteSheet;
	while(!in.eof()) {
		in.getline(input, 256);
		data = strtrimcpy(input);
		
		if(strlen(data) == 0) {
			delete [] data;
			continue;
		}
		else {
			Variable * v = new Variable;
			if(v->parseFrom(data)) {
				delete [] data;
				properties.addCopy(v);
				delete v;
			}
			else {
				delete v;
				delete [] data;
				continue;
			}

			v = properties.getVariable("Attributes");
			if(v == NULL) { v = properties.getVariable("Number of Sprites"); }

			if(v != NULL) {
				spriteSheetName = strtrimcpy(properties.getValue("SpriteSheet Name"));
				if(spriteSheetName == NULL) {
					printf("ERROR: Sprite sheet must have a name.\n");
					return NULL;
				}
				
				spriteSheetType = atoi(properties.getValue("SpriteSheet Type"));

				spriteSheetFileName = strtrimcpy(properties.getValue("File Name"));
				if(spriteSheetFileName == NULL) {
					delete [] spriteSheetName;
					printf("ERROR: Sprite sheet must have a name.\n");
					return NULL;
				}

				spriteSheetImage = new Sprite(spriteSheetFileName, spriteDirectory, d3dDevice);
				spriteSheetImage->setType(Sprite::TYPE_SHEET);
				delete [] spriteSheetFileName;

				// Arbitrary Offset SpriteSheet Type ==============================================
				if(spriteSheetType == 1) {
					int numberOfSprites;
					
					if(v->value() == NULL) {
						printf("ERROR: Number of sprites specification is missing from current sprite sheet.\n");
						delete [] spriteSheetName;
						delete spriteSheetImage;
						return NULL;
					}
					numberOfSprites = atoi(v->value());
					if(numberOfSprites <= 0) {
						printf("ERROR: Must parse at least 1 sprite from sprite sheet.\n");
						delete [] spriteSheetName;
						delete spriteSheetImage;
						return NULL;
					}

					int spriteIndex;
					char * spriteName;
					int spriteType;
					int xOffset;
					int yOffset;
					int width;
					int height;
					SpriteSheetOffset * offsets = new SpriteSheetOffset[numberOfSprites];
					char ** spriteNames = new char*[numberOfSprites];
					int * spriteTypes = new int[numberOfSprites];
					Variables spriteAttributes;
					for(int i=0;i<numberOfSprites;i++) {
						spriteNames[i] = NULL;
						spriteTypes[i] = Sprite::TYPE_UNKNOWN;
						offsets[i].x = 0;
						offsets[i].y = 0;
						offsets[i].w = 0;
						offsets[i].h = 0;
					}
					for(int i=0;i<numberOfSprites;i++) {
						Variable * newVariable;
						for(int j=0;j<5;j++) {
							newVariable = new Variable;
							in.getline(input, 256);
							char * tempData = strtrimcpy(input);
							if(newVariable->parseFrom(tempData)) {
								spriteAttributes.add(newVariable);
							}
							else {
								delete newVariable;
							}
							delete [] tempData;
						}
						
						if(spriteAttributes.getValue("Sprite") == NULL) {
							printf("ERROR: Sprite index specification is missing from current sprite.\n");
							delete [] spriteSheetName;
							delete spriteSheetImage;
							delete [] spriteNames;
							delete [] spriteTypes;
							return NULL;
						}
						spriteIndex = atoi(spriteAttributes.getValue("Sprite"));
						if(spriteIndex < 0 || spriteIndex >= numberOfSprites) {
							printf("ERROR: Current sprite index out of bounds.");
							delete [] spriteSheetName;
							delete spriteSheetImage;
							delete [] spriteNames;
							delete [] spriteTypes;
							return NULL;
						}

						spriteName = strtrimcpy(spriteAttributes.getValue("Name"));
						if(spriteName == NULL) {
							printf("ERROR: Name missing from current sprite.");
							delete [] spriteSheetName;
							delete spriteSheetImage;
							for(int j=0;j<numberOfSprites;j++) {
								if(spriteNames[j] != NULL) {
									delete [] spriteNames[j];
								}
							}
							delete [] spriteNames;
							delete [] spriteName;
							delete [] spriteTypes;
							return NULL;
						}

						if(spriteAttributes.getValue("Type") == NULL) {
							printf("ERROR: Type missing from current sprite.");
							delete [] spriteSheetName;
							delete spriteSheetImage;
							for(int j=0;j<numberOfSprites;j++) {
								if(spriteNames[j] != NULL) {
									delete [] spriteNames[j];
								}
							}
							delete [] spriteNames;
							delete [] spriteName;
							delete [] spriteTypes;
							return NULL;
						}
						spriteType = Sprite::parseType(spriteAttributes.getValue("Type"));

						char * offsetValue = strtrimcpy(spriteAttributes.getValue("Offset"));
						if(offsetValue == NULL) {
							printf("ERROR: Offset missing from current sprite.");
							delete [] spriteSheetName;
							delete spriteSheetImage;
							for(int j=0;j<numberOfSprites;j++) {
								if(spriteNames[j] != NULL) {
									delete [] spriteNames[j];
								}
							}
							delete [] spriteNames;
							delete [] spriteName;
							delete [] spriteTypes;
							delete [] offsetValue;
							return NULL;
						}
						char * offsetCenter = strchr(offsetValue, ',');

						char * sizeValue = strtrimcpy(spriteAttributes.getValue("Size"));
						if(sizeValue == NULL) {
							printf("ERROR: Size missing from current sprite.");
							delete [] spriteSheetName;
							delete spriteSheetImage;
							for(int j=0;j<numberOfSprites;j++) {
								if(spriteNames[j] != NULL) {
									delete [] spriteNames[j];
								}
							}
							delete [] spriteNames;
							delete [] spriteTypes;
							delete [] spriteName;
							delete [] offsetValue;
							delete [] sizeValue;
							return NULL;
						}
						char * sizeCenter = strchr(sizeValue, ',');

						*offsetCenter = '\0';
						*sizeCenter = '\0';
						xOffset = atoi(offsetValue);
						yOffset = atoi(offsetCenter + sizeof(char));
						width = atoi(sizeValue);
						height = atoi(sizeCenter + sizeof(char));

						delete [] offsetValue;
						delete [] sizeValue;

						spriteNames[spriteIndex] = spriteName;
						spriteTypes[spriteIndex] = spriteType;
						offsets[spriteIndex].x = xOffset;
						offsets[spriteIndex].y = yOffset;
						offsets[spriteIndex].w = width;
						offsets[spriteIndex].h = height;

						spriteAttributes.clear(true);
					}

					spriteSheet = new SpriteSheet(spriteSheetImage, offsets, numberOfSprites);
					spriteSheet->setName(spriteSheetName);

					for(int i=0;i<numberOfSprites;i++) {
						spriteSheet->getSprite(i)->setIndex(i);
						spriteSheet->getSprite(i)->setName(spriteNames[i]);
						spriteSheet->getSprite(i)->setParentName(spriteSheetName);
						spriteSheet->getSprite(i)->setType(spriteTypes[i]);
					}

					delete [] spriteSheetName;

					for(int i=0;i<numberOfSprites;i++) {
						if(spriteNames[i] != NULL) {
							delete [] spriteNames[i];
						}
					}
					delete [] spriteNames;
					delete [] spriteTypes;
					delete [] offsets;
				}
				// Single Grid SpriteSheet Type ===============================================================
				else if(spriteSheetType == TYPE_SINGLE_GRID) {
					int numberOfAttributes;
					int xOffset, yOffset;
					int width, height;
					int xIncrement, yIncrement;
					bool horizontal;
					int numberOfRows, numberOfColumns;

					numberOfAttributes = atoi(v->value());

					char * offsetValue = strtrimcpy(properties.getValue("Offset"));
					char * sizeValue = strtrimcpy(properties.getValue("Size"));
					char * incrementValue = strtrimcpy(properties.getValue("Increment"));
					char * horizontalValue = strtrimcpy(properties.getValue("Horizontal"));
					char * numberOfRowsValue = properties.getValue("Number of Rows");
					char * numberOfColumnsValue = properties.getValue("Number of Columns");

						 if(offsetValue == NULL)		 { printf("ERROR: Missing sprite sheet offset.\n"); }
					else if(sizeValue == NULL)			 { printf("ERROR: Missing sprite sheet tile size.\n"); }
					else if(incrementValue == NULL)		 { printf("ERROR: Missing sprite sheet tile increment.\n"); }
					else if(horizontalValue == NULL)	 { printf("ERROR: Missing horizontal / vertical sprite sheet specification.\n"); }
					else if(numberOfRowsValue == NULL)	 { printf("ERROR: Missing number of rows specification in sprite sheet.\n"); }
					else if(numberOfColumnsValue == NULL){ printf("ERROR: Missing number of columns specification in sprite sheet.\n"); }
					if(offsetValue == NULL || sizeValue == NULL || incrementValue == NULL || horizontalValue == NULL) {
						delete [] spriteSheetName;
						delete [] spriteSheetFileName;
						delete spriteSheetImage;
						delete [] offsetValue;
						delete [] sizeValue;
						delete [] incrementValue;
						delete [] horizontalValue;
						return NULL;
					}

					char * offsetCenter = strchr(offsetValue, ',');
					char * sizeCenter = strchr(sizeValue, ',');
					char * incrementCenter = strchr(incrementValue, ',');
					horizontal = isTrue(horizontalValue);
					delete [] horizontalValue;
					
						 if(offsetCenter == NULL)	{ printf("ERROR: Invalid sprite sheet offset.\n"); }
					else if(sizeCenter == NULL)		{ printf("ERROR: Invalid sprite sheet tile size.\n"); }
					else if(incrementCenter == NULL){ printf("ERROR: Invalid sprite sheet tile increment.\n"); }
					if(offsetCenter == NULL || sizeCenter == NULL || incrementCenter == NULL) {
						delete [] spriteSheetName;
						delete [] spriteSheetFileName;
						delete spriteSheetImage;
						delete [] offsetValue;
						delete [] sizeValue;
						delete [] incrementValue;
						return NULL;
					}
					
					*offsetCenter = '\0';
					*sizeCenter = '\0';
					*incrementCenter = '\0';
					xOffset = atoi(offsetValue);
					yOffset = atoi(offsetCenter + sizeof(char));
					width = atoi(sizeValue);
					height = atoi(sizeCenter + sizeof(char));
					xIncrement = atoi(incrementValue);
					yIncrement = atoi(incrementCenter + sizeof(char));
					numberOfRows = atoi(numberOfRowsValue);
					numberOfColumns = atoi(numberOfColumnsValue);

					delete [] offsetValue;
					delete [] sizeValue;
					delete [] incrementValue;
					
					spriteSheet = new SpriteSheet(spriteSheetImage,
												  xOffset,
												  yOffset,
												  width,
												  height,
												  xIncrement,
												  yIncrement,
												  horizontal,
												  numberOfRows,
												  numberOfColumns);
					spriteSheet->setName(spriteSheetName);

					delete [] spriteSheetName;

					int spriteIndex;
					char * spriteName;
					int spriteType;
					Variables spriteAttributes;
					for(int i=0;i<numberOfAttributes;i++) {
						for(int j=0;j<3;j++) {
							in.getline(input, 256);
							Variable * newVariable = new Variable;
							if(newVariable->parseFrom(input)) {
								spriteAttributes.add(newVariable);
							}
						}

						char * spriteIndexValue = spriteAttributes.getValue("Sprite");
						if(spriteIndexValue == NULL) {
							printf("WARNING: Sprite index attribute missing.\n");
							return spriteSheet;
						}
						spriteIndex = atoi(spriteIndexValue);
						if(spriteIndex < 0 || spriteIndex >= spriteSheet->size()) {
							printf("WARNING: Sprite index attribute out of range.\n");
							return spriteSheet;
						}

						spriteName = spriteAttributes.getValue("Name");
						if(spriteName == NULL) {
							printf("WARNING: Sprite name attribute missing.\n");
							return spriteSheet;
						}

						char * spriteTypeValue = spriteAttributes.getValue("Type");
						if(spriteTypeValue == NULL) {
							printf("WARNING: Sprite type attribute missing.\n");
							return spriteSheet;
						}
						spriteType = Sprite::parseType(spriteTypeValue);

						spriteSheet->sprites.at(spriteIndex)->setName(spriteName);
						spriteSheet->sprites.at(spriteIndex)->setIndex(spriteIndex);
						spriteSheet->sprites.at(spriteIndex)->setParentName(spriteSheetName);
						spriteSheet->sprites.at(spriteIndex)->setType(spriteType);

						spriteAttributes.clear(true);
					}
				}
				else if(spriteSheetType == TYPE_MULTIPLE_GRIDS) {
					prompt("TODO: Finish Multiple Grid SpriteSheet Import Routine.");
					return NULL;
				}
				// Invalid SpriteSheet Type ===================================================================
				else {
					printf("ERROR: Invalid sprite sheet type specified.\n");
					delete [] spriteSheetName;
					delete [] spriteSheetFileName;
					delete spriteSheetImage;
					return NULL;
				}

				return spriteSheet;
			}
			
		}

	}

	return NULL;
}
