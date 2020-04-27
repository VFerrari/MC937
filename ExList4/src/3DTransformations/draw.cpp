#include "draw.h"

Scene::Scene(uint16_t width, uint16_t height, GLuint *vbo, GLuint *vao){
    this->vbo = vbo;
    this->vao = vao;
    createFigures();
    createCamera(width, height);
}

void Scene::createFigures(void){
    // Pyramid (equilateral)
    drawPyramid(2.f, 2.f, 2.f, -6.f, 3.f, -1.f);
    findCenter(curr_shape);
    
    // Pyramid (tall)
    curr_shape++;
    drawPyramid(1.f, 7.f, 2.f, 9.f, -10.f, -5.f);
    findCenter(curr_shape);

    // Pyramid (flat)
    curr_shape++;
    drawPyramid(2.f, 0.3f, 5.f, 2.f, 3.f, 10.f);
    findCenter(curr_shape);

    // Cube (regular)
    curr_shape++;
    drawParallelepiped(2.f, 2.f, 2.f, -1.f, -1.f, -1.f);
    findCenter(curr_shape);
    
    // Cube (tiny)
    curr_shape++;
    drawParallelepiped(0.2f, 0.2f, 0.2f, -0.8f, -5.f, 4.f);
    findCenter(curr_shape); 
    
    // Parallelepiped (tall)
    curr_shape++;
    drawParallelepiped(1.f, 3.f, 2.f, 6.5f, -2.f, -1.f);
    findCenter(curr_shape);
    
    // Parallelepiped (wide)
    curr_shape++;
    drawParallelepiped(4.f, 2.f, 1.f, -8.f, -7.f, -6.f);
    findCenter(curr_shape);
    
    // Hourglass
    curr_shape++;
    drawHourglass(2.f, 4.f, 2.f, 3.f, -5.f, 0.f);
    findCenter(curr_shape);
    
    // D20 (icosahedron)
    curr_shape++;
    
    curr_shape = 0;
}

void Scene::createCamera(uint16_t width, uint16_t height){
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    //model      = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width)/height, 0.1f, 100.0f);
    
    camera_pos = projection*view*model;
}

void Scene::moveCamera(float factorX, float factorY){
    camera_pos = glm::translate(camera_pos, glm::vec3(factorX, factorY, 0.0f));
}

glm::mat4 Scene::getCamera(void){
    return camera_pos;
}

bool Scene::getCameraMode(void){
    return camera_mode;
}

void Scene::setCameraMode(void){
    camera_mode = !camera_mode;
}

void Scene::setCurrentShape(uint8_t shape){
    curr_shape = shape;
}

uint8_t Scene::getShape(uint8_t index, std::vector<float> *dest){
    if (index >= SHAPES){
        std::cerr << "Shape index out of range!\n" << std::endl;
        return 0;
    }

    *dest = shapes[index];
    return 1;
}

uint8_t Scene::getCurrentShape(){
    return curr_shape;
}

/**
 * Random number between 0 and 1. Used for color generation.
 */
float getRandomColor(void){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

/**
 * Draws a pyramid in the current shape, given info on it.
 * @param start_x : leftmost X value
 * @param start_y : bottom y value
 * @param start_z : farthest z value
 */
void Scene::drawPyramid(float width, float height, float depth, float start_x, float start_y, float start_z){
    std::vector<float> colors[5];
    float peak_x = start_x + width/2;
    float peak_z = start_z + depth/2;
    float peak_y = start_y+height;
    
    // Generate colors
    for(int i=0; i<5; i++)
        colors[i] = {getRandomColor(), getRandomColor(), getRandomColor()};
    
    shapes[curr_shape] = {
        
        // Faces
        peak_x,        peak_y,  peak_z,        colors[0][0], colors[0][1], colors[0][2],
        start_x,       start_y, start_z+depth, colors[0][0], colors[0][1], colors[0][2],
        start_x+width, start_y, start_z+depth, colors[0][0], colors[0][1], colors[0][2],
        
        peak_x,        peak_y,  peak_z,        colors[1][0], colors[1][1], colors[1][2],
        start_x+width, start_y, start_z+depth, colors[1][0], colors[1][1], colors[1][2],
        start_x+width, start_y, start_z,       colors[1][0], colors[1][1], colors[1][2],

        peak_x,        peak_y,  peak_z,        colors[2][0], colors[2][1], colors[2][2],
        start_x,       start_y, start_z,       colors[2][0], colors[2][1], colors[2][2],
        start_x+width, start_y, start_z,       colors[2][0], colors[2][1], colors[2][2],
        
        peak_x,        peak_y,  peak_z,        colors[3][0], colors[3][1], colors[3][2],
        start_x,       start_y, start_z,       colors[3][0], colors[3][1], colors[3][2],
        start_x,       start_y, start_z+depth, colors[3][0], colors[3][1], colors[3][2],
        
        // Base
        start_x,       start_y, start_z+depth, colors[4][0], colors[4][1], colors[4][2],
        start_x,       start_y, start_z,       colors[4][0], colors[4][1], colors[4][2],
        start_x+width, start_y, start_z+depth, colors[4][0], colors[4][1], colors[4][2],

        start_x+width, start_y, start_z+depth, colors[4][0], colors[4][1], colors[4][2],
        start_x+width, start_y, start_z,       colors[4][0], colors[4][1], colors[4][2],
        start_x,       start_y, start_z,       colors[4][0], colors[4][1], colors[4][2]
    };
}

/**
 * Draws a parallelepiped in the current shape, given info on it.
 * @param start_x : leftmost X value
 * @param start_y : bottom y value
 * @param start_z : farthest z value
 */
void Scene::drawParallelepiped(float width, float height, float depth, float start_x, float start_y, float start_z){
    std::vector<float> colors[6];
    
    // Generate colors
    for(int i=0; i<6; i++)
        colors[i] = {getRandomColor(), getRandomColor(), getRandomColor()};
    
    shapes[curr_shape] = {
        start_x,       start_y,        start_z,       colors[0][0], colors[0][1], colors[0][2], // Left face
        start_x,       start_y,        start_z+depth, colors[0][0], colors[0][1], colors[0][2],
        start_x,       start_y+height, start_z+depth, colors[0][0], colors[0][1], colors[0][2],
        
        start_x+width, start_y+height, start_z,       colors[1][0], colors[1][1], colors[1][2], // Far face
        start_x,       start_y,        start_z,       colors[1][0], colors[1][1], colors[1][2], 
        start_x,       start_y+height, start_z,       colors[1][0], colors[1][1], colors[1][2], 
        
        start_x+width, start_y       , start_z+depth, colors[2][0], colors[2][1], colors[2][2], // Lower face
        start_x      , start_y       , start_z,       colors[2][0], colors[2][1], colors[2][2],
        start_x+width, start_y       , start_z,       colors[2][0], colors[2][1], colors[2][2],
        
        start_x+width, start_y+height, start_z,       colors[1][0], colors[1][1], colors[1][2], // Far face
        start_x+width, start_y       , start_z,       colors[1][0], colors[1][1], colors[1][2], 
        start_x,       start_y,        start_z,       colors[1][0], colors[1][1], colors[1][2], 
        
        start_x,       start_y+height, start_z,       colors[0][0], colors[0][1], colors[0][2], // Left face
        start_x,       start_y+height, start_z+depth, colors[0][0], colors[0][1], colors[0][2],
        start_x,       start_y,        start_z,       colors[0][0], colors[0][1], colors[0][2],
        
        start_x+width, start_y       , start_z+depth, colors[2][0], colors[2][1], colors[2][2], // Lower face
        start_x,       start_y,        start_z+depth, colors[2][0], colors[2][1], colors[2][2],
        start_x,       start_y,        start_z,       colors[2][0], colors[2][1], colors[2][2],
        
        start_x,       start_y+height, start_z+depth, colors[3][0], colors[3][1], colors[3][2], // Close face
        start_x,       start_y,        start_z+depth, colors[3][0], colors[3][1], colors[3][2],
        start_x+width, start_y       , start_z+depth, colors[3][0], colors[3][1], colors[3][2],
        
        start_x+width, start_y+height, start_z+depth, colors[4][0], colors[4][1], colors[4][2], // Right face
        start_x+width, start_y       , start_z,       colors[4][0], colors[4][1], colors[4][2],
        start_x+width, start_y+height, start_z,       colors[4][0], colors[4][1], colors[4][2],
        
        start_x+width, start_y       , start_z,       colors[4][0], colors[4][1], colors[4][2], // Right face
        start_x+width, start_y+height, start_z+depth, colors[4][0], colors[4][1], colors[4][2],
        start_x+width, start_y       , start_z+depth, colors[4][0], colors[4][1], colors[4][2],
        
        start_x+width, start_y+height, start_z+depth, colors[5][0], colors[5][1], colors[5][2], // Upper face
        start_x+width, start_y+height, start_z,       colors[5][0], colors[5][1], colors[5][2],
        start_x,       start_y+height, start_z,       colors[5][0], colors[5][1], colors[5][2],
        
        start_x+width, start_y+height, start_z+depth, colors[5][0], colors[5][1], colors[5][2], // Upper face
        start_x,       start_y+height, start_z,       colors[5][0], colors[5][1], colors[5][2],
        start_x,       start_y+height, start_z+depth, colors[5][0], colors[5][1], colors[5][2],
        
        start_x+width, start_y+height, start_z+depth, colors[3][0], colors[3][1], colors[3][2], // Close face
        start_x,       start_y+height, start_z+depth, colors[3][0], colors[3][1], colors[3][2],
        start_x+width, start_y       , start_z+depth, colors[3][0], colors[3][1], colors[3][2]
    };
}

/**
 * Draws a hourglass in the current shape, given info on it.
 * Two pyramids connected by their peaks.
 * @param start_x : leftmost X value
 * @param start_y : bottom y value
 * @param start_z : farthest z value
 */
void Scene::drawHourglass(float width, float height, float depth, float start_x, float start_y, float start_z){
    std::vector<float> colors[10];
    float middle_x = start_x + width/2;
    float middle_z = start_z + depth/2;
    float middle_y = start_y + height/2;
    
    // Generate colors
    for(int i=0; i<10; i++)
        colors[i] = {getRandomColor(), getRandomColor(), getRandomColor()};

    shapes[curr_shape] = {
        // Base of the top pyramid
        start_x,       start_y+height, start_z+depth, colors[0][0], colors[0][1], colors[0][2],
        start_x,       start_y+height, start_z,       colors[0][0], colors[0][1], colors[0][2],
        start_x+width, start_y+height, start_z+depth, colors[0][0], colors[0][1], colors[0][2],

        start_x+width, start_y+height, start_z+depth, colors[0][0], colors[0][1], colors[0][2],
        start_x+width, start_y+height, start_z,       colors[0][0], colors[0][1], colors[0][2],
        start_x,       start_y+height, start_z,       colors[0][0], colors[0][1], colors[0][2],

        // Faces of the top pyramid.
        start_x,       start_y+height, start_z+depth, colors[2][0], colors[2][1], colors[2][2],
        start_x+width, start_y+height, start_z+depth, colors[2][0], colors[2][1], colors[2][2],
        middle_x,      middle_y,       middle_z,      colors[2][0], colors[2][1], colors[2][2],
        
        start_x+width, start_y+height, start_z+depth, colors[3][0], colors[3][1], colors[3][2],
        start_x+width, start_y+height, start_z,       colors[3][0], colors[3][1], colors[3][2],
        middle_x,      middle_y,       middle_z,      colors[3][0], colors[3][1], colors[3][2],

        start_x,       start_y+height, start_z,       colors[4][0], colors[4][1], colors[4][2],
        start_x+width, start_y+height, start_z,       colors[4][0], colors[4][1], colors[4][2],
        middle_x,      middle_y,       middle_z,      colors[4][0], colors[4][1], colors[4][2],        
        
        start_x,       start_y+height, start_z,       colors[5][0], colors[5][1], colors[5][2],
        start_x,       start_y+height, start_z+depth, colors[5][0], colors[5][1], colors[5][2],
        middle_x,      middle_y,       middle_z,      colors[5][0], colors[5][1], colors[5][2],
        
        // Faces of the bottom pyramid.
        middle_x,      middle_y,middle_z,      colors[6][0], colors[6][1], colors[6][2],
        start_x,       start_y, start_z+depth, colors[6][0], colors[6][1], colors[6][2],
        start_x+width, start_y, start_z+depth, colors[6][0], colors[6][1], colors[6][2],
        
        middle_x,      middle_y,middle_z,      colors[7][0], colors[7][1], colors[7][2],
        start_x+width, start_y, start_z+depth, colors[7][0], colors[7][1], colors[7][2],
        start_x+width, start_y, start_z,       colors[7][0], colors[7][1], colors[7][2],

        middle_x,      middle_y,middle_z,      colors[8][0], colors[8][1], colors[8][2],
        start_x,       start_y, start_z,       colors[8][0], colors[8][1], colors[8][2],
        start_x+width, start_y, start_z,       colors[8][0], colors[8][1], colors[8][2],
        
        middle_x,      middle_y,middle_z,      colors[9][0], colors[9][1], colors[9][2],
        start_x,       start_y, start_z,       colors[9][0], colors[9][1], colors[9][2],
        start_x,       start_y, start_z+depth, colors[9][0], colors[9][1], colors[9][2],
        
        // Base of the bottom pyramid
        start_x,       start_y, start_z+depth, colors[1][0], colors[1][1], colors[1][2],
        start_x,       start_y, start_z,       colors[1][0], colors[1][1], colors[1][2],
        start_x+width, start_y, start_z+depth, colors[1][0], colors[1][1], colors[1][2],

        start_x+width, start_y, start_z+depth, colors[1][0], colors[1][1], colors[1][2],
        start_x+width, start_y, start_z,       colors[1][0], colors[1][1], colors[1][2],
        start_x,       start_y, start_z,       colors[1][0], colors[1][1], colors[1][2]
    };
}

void Scene::bufferShape(uint8_t shape){
    std::vector<float> points;
        
    // Get shape.
    getShape(shape, &points);
    
    // Bind a VBO and initialize data.
    glBindBuffer(GL_ARRAY_BUFFER, vbo[shape]);
    glBufferData(GL_ARRAY_BUFFER, points.size()*6*sizeof(GLfloat), points.data(), GL_STATIC_DRAW); 

    // Bind a VAO, and set its attributes.
    glBindVertexArray(vao[shape]);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr); 
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<const void *>(3*sizeof(float))); 
}


/* TRANSFORMATIONS */
void Scene::scaleOneShape(float factorX, float factorY, float factorZ, uint8_t shape){
    int i;
    
    for(i=0; i<shapes[shape].size(); i+=3){
        shapes[shape][i++] *= factorX;
        shapes[shape][i++] *= factorY;
        shapes[shape][i++] *= factorZ;
    }
}

void Scene::translateOneShape(float factorX, float factorY, float factorZ, uint8_t shape){
    int i;
    
    for(i=0; i<shapes[shape].size(); i+=3){
        shapes[shape][i++] += factorX;
        shapes[shape][i++] += factorY;
        shapes[shape][i++] += factorZ;
    }
}

void Scene::rotateOneShape(int angle, uint8_t axis, uint8_t shape){
    double sin = std::sin(angle*PI/180);
    double cos = std::cos(angle*PI/180);
    float aux;
    int i;
    
    if(axis == 1){
        
        // X-axis
        for(i=0; i<shapes[shape].size(); i+=6){
            aux = shapes[shape][i+1]*sin + shapes[shape][i+2]*cos;
            shapes[shape][i+1] = shapes[shape][i+1]*cos - shapes[shape][i+2]*sin;
            shapes[shape][i+2] = aux;
        }
    }
    else if (axis == 2){
        
        // Y-axis
        for(i=0; i<shapes[shape].size(); i+=6){
            aux = shapes[shape][i]*sin + shapes[shape][i+2]*cos;
            shapes[shape][i] = shapes[shape][i]*cos - shapes[shape][i+2]*sin;
            shapes[shape][i+2] = aux;
        }
    }
    else{
        
        // Z-axis
        for(i=0; i<shapes[shape].size(); i+=6){
            aux = shapes[shape][i]*sin + shapes[shape][i+1]*cos;
            shapes[shape][i] = shapes[shape][i]*cos - shapes[shape][i+1]*sin;
            shapes[shape][i+1] = aux;
        }
    }
}

void Scene::scale(float factorX, float factorY, float factorZ){
    int i;
    
    if(curr_shape < SHAPES){
        translateOneShape(-center[curr_shape][0],-center[curr_shape][1], -center[curr_shape][2], curr_shape);
        scaleOneShape(factorX, factorY, factorZ, curr_shape);
        translateOneShape(center[curr_shape][0],center[curr_shape][1], center[curr_shape][2], curr_shape);
        findCenter(curr_shape);
    }
    else{
        for(i=0; i<SHAPES; i++){
            scaleOneShape(factorX, factorY, factorZ, i);
            findCenter(i);
        }
    }
}


void Scene::translate(float factorX, float factorY, float factorZ){
    int i;
    
    if(curr_shape < SHAPES){
        translateOneShape(factorX, factorY, factorZ, curr_shape);
        center[curr_shape][0] += factorX;
        center[curr_shape][1] += factorY;
        center[curr_shape][2] += factorZ;
    }
    else{
        for(i=0; i<SHAPES; i++){
            translateOneShape(factorX, factorY, factorZ, i);
            center[i][0] += factorX;
            center[i][1] += factorY;
            center[i][2] += factorZ;
        }
    }
}

/**
 * Rotates shape or scene.
 * If to rotate shape, translates to origin, rotates and then back to original spot.
 * Or just rotate the entire scene in relation to the origin. Then find the new center of each.
 */
void Scene::rotate(int angle, uint8_t axis){
    int i;
    
    if(curr_shape < SHAPES){
        translateOneShape(-center[curr_shape][0],-center[curr_shape][1], -center[curr_shape][2], curr_shape);
        rotateOneShape(angle, axis, curr_shape);
        translateOneShape(center[curr_shape][0],center[curr_shape][1], center[curr_shape][2], curr_shape);
    }
    else{
        for(i=0; i<SHAPES; i++){
            rotateOneShape(angle, axis, i);
            findCenter(i);
        }
    }
}

/**
 * Finds center of bounding box.
 * For transformations in local coordinates.
 * @param shape: which shape to find center.
 */
void Scene::findCenter(uint8_t shape){
    int i, j;
    float max[3] = {-100000.f, -100000.f, -100000.f};
    float min[3] = {100000.f, 100000.f, 100000.f};
    
    for(i=0; i<shapes[shape].size(); i+=6){
        for(j=0; j<3; j++){
            max[j] = std::max(shapes[shape][i+j], max[j]);
            min[j] = std::min(shapes[shape][i+j], min[j]);
        }
    }
    for(i=0; i<3; i++)
        center[shape][i] = (max[i] + min[i])/2;
}
