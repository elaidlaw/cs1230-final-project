#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"
#include "shapes/implicitsurfaceaggregation.h"

#include <glm/gtx/transform.hpp>
#include <cmath>


RayScene::RayScene(Scene &scene) :
    Scene(scene),
    m_implicitshapes(std::vector<std::shared_ptr<ImplicitShape>>()),
    m_maxrecursiondepth(6)
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.

    for (int i = 0; i < static_cast<int>(m_primitives.size()); i++){
        m_implicitshapes.push_back(std::make_shared<ImplicitSurfaceAggregation>(ImplicitSurfaceAggregation(m_primitives[i]->type)));
    }

    for (int i = 0; i < static_cast<int>(m_transforms.size()); i++){
        std::shared_ptr<glm::mat4x4> invMat = std::make_shared<glm::mat4x4>(glm::inverse(*(m_transforms[i])));
        m_inverseTransforms.push_back(invMat);
//        std::shared_ptr<glm::mat4x4> invRot = std::make_shared<glm::mat4x4>(glm::mat4x4(glm::mat3x3(*invMat)));
//        m_inverseRotations.push_back(invRot);
    }

}

RayScene::~RayScene()
{
}

std::vector<glm::vec4> RayScene::rayTrace(int width, int height){
    //get start and direction of each ray
    std::vector<glm::vec4> viewPlanePoints = viewPlanePointsWorldSpace(width, height);
    glm::vec4 point = m_cameraData->pos;

    //vector to store colors in
    std::vector<glm::vec4> channelColors = std::vector<glm::vec4>();

    for (int i = 0; i < static_cast<int>(viewPlanePoints.size()); i++){
        glm::vec4 channelColor = rayLighting(point, viewPlanePoints[i], 0);
        channelColors.push_back(channelColor);
    }
    return channelColors;

}

std::vector<glm::vec4> RayScene::viewPlanePointsCameraSpace(int width, int height){
    std::vector<glm::vec4> rays = std::vector<glm::vec4>();

    //calculate the width (U) and height (V) of the view plane
    //arbitrarily set plane depth to be 1.0
    double k = 1.0;
    double V = 2*k*tan(glm::radians(m_cameraData->heightAngle) / 2.0);
    double U = V / height * width;

    for (int i = height-1; i >= 0; i--){
        for (int j = 0; j < width; j++){
            glm::vec4 viewPlanePoint = glm::vec4(((j+0.5)/width-0.5)*U,((i+0.5)/height-0.5)*V,-k,1.0);
            rays.push_back(viewPlanePoint);
        }
    }
    return rays;
}

std::vector<glm::vec4> RayScene::viewPlanePointsWorldSpace(int width, int height){
    std::vector<glm::vec4> rays = viewPlanePointsCameraSpace(width, height);

//    //calculate the camera to world transform
//    glm::vec3 worldLook = glm::vec3(m_cameraData->look);
//    worldLook = glm::normalize(worldLook);
//    glm::vec3 camLook = glm::vec3(0.0,0.0,-1.0);
//    double cosTheta = glm::dot(camLook, worldLook)/(glm::length(camLook), glm::length(worldLook));
//    double sinTheta = glm::length(glm::cross(camLook, worldLook))/(glm::length(camLook), glm::length(worldLook));
//    glm::vec3 rotationAxis = glm::cross(camLook, worldLook);

//    //we have the sine and cosine of the angle, now we can find the angle itself using arcTan (Have to handle divide by zero manually)
//    double angle = atan2(sinTheta, cosTheta);
//    //glm::mat4x4 camToWorld = glm::rotate(static_cast<float>(angle), rotationAxis);


    glm::vec3 look = glm::vec3(m_cameraData->look);
    glm::vec3 up = glm::vec3(m_cameraData->up);
    glm::vec3 w = -look;
    w = glm::normalize(w);
    glm::vec3 v = up - glm::dot(up, w)*w;
    v = glm::normalize(v);
    glm::vec3 u = glm::cross(v,w);

    glm::mat4x4 rotationMatrix = glm::mat4x4(u[0], v[0], w[0], 0.0, u[1], v[1], w[1], 0.0, u[2], v[2], w[2], 0.0, 0.0, 0.0, 0.0, 1.0);
    glm::mat4x4 translationMatrix = glm::mat4x4(1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,-m_cameraData->pos[0], -m_cameraData->pos[1], -m_cameraData->pos[2], 1.0);
    glm::mat4x4 camToWorld = rotationMatrix*translationMatrix;
    camToWorld = glm::inverse(camToWorld);

    //transform all of the cam space directions to world space directions
    std::vector<glm::vec4> worldRays = std::vector<glm::vec4>();
    for(int i = 0; i < static_cast<int>(rays.size()); i++){
        worldRays.push_back(camToWorld*rays[i]);
    }
    return worldRays;
}

std::tuple<double, int> RayScene::getTValue(glm::vec4 point, glm::vec4 viewPlanePoint){
    double t = -1.0;
    int objectIndex = -1;

    //check for intersection with every object in scene, choose the closest object
    for (int i = 0; i < static_cast<int>(m_implicitshapes.size()); i++){
        glm::vec3 objSpacePoint = glm::vec3(*m_inverseTransforms[i]*point);
        glm::vec3 objSpaceviewPlanePoint = glm::vec3(*m_inverseTransforms[i]*viewPlanePoint);
        glm::vec3 objSpaceDirection = objSpaceviewPlanePoint - objSpacePoint;
//        objSpaceDirection = glm::normalize(objSpaceDirection);
        double shapeT = m_implicitshapes[i]->getTValue(objSpacePoint, objSpaceDirection);
        if (shapeT > 0.0){
            if (shapeT < t || t < 0.0){
                t = shapeT;
                objectIndex = i;
            }
        }
    }

    return std::tuple<double, int>(t, objectIndex);
}

glm::vec4 RayScene::getPosition(glm::vec4 point, glm::vec4 viewPlanePoint, double t){
    glm::vec4 direction = viewPlanePoint - point;
    glm::vec4 finalPosition = point + direction*static_cast<float>(t);
    return finalPosition;
}

glm::vec4 RayScene::getNormal(glm::vec4 position, int objectIndex){
    position = *m_inverseTransforms[objectIndex]*position;
    glm::vec3 v3normal = m_implicitshapes[objectIndex]->getNormal(position[0], position[1], position[2]);
//    glm::vec4 v4normal = glm::vec4(v3normal, 1.0);
    //TODO: Adjust normal according to transformation
    glm::mat3x3 normalTransform = glm::mat3x3(*m_transforms[objectIndex]);
    normalTransform = glm::transpose(normalTransform);
    normalTransform = glm::inverse(normalTransform);
    v3normal = normalTransform*v3normal;
    return glm::vec4(v3normal, 1.0);
}

glm::vec4 RayScene::QrgbToVec4(QRgb color){
    return glm::vec4(static_cast<double>(qRed(color))/255.0, static_cast<double>(qGreen(color))/255.0, static_cast<double>(qBlue(color))/255.0,0.0);
}

glm::vec4 RayScene::reflectRay(glm::vec4 ray, glm::vec4 normal){
    ray = glm::vec4(glm::normalize(glm::vec3(ray)), 0.0);
    normal = glm::vec4(glm::normalize(glm::vec3(normal)), 0.0);
    glm::vec4 reflectedLightDirection = 2.f * normal * glm::dot(normal, ray) - ray;
    reflectedLightDirection = glm::vec4(glm::normalize(glm::vec3(reflectedLightDirection)), 0.0);
    return reflectedLightDirection;
}

glm::vec4 RayScene::computeTextureColor(glm::vec4 position, int objectIndex){
    /* Returns the texture color on the object surface for a position provided in world space
     * The relevant primitive is saved in member variables.
     */
    if (m_primitiveTextures[objectIndex]->isNull()){
        return glm::vec4(0.0);
    }
    //convert the position to object space
    position = *m_inverseTransforms[objectIndex]*position;
    glm::vec2 uv = m_implicitshapes[objectIndex]->getUV(position[0], position[1], position[2]);
    int textureWidth = m_primitiveTextures[objectIndex]->width();
    int textureHeight = m_primitiveTextures[objectIndex]->height();

    float pixelXFloat = static_cast<float>(textureWidth*m_primitives[objectIndex]->material.textureMap.repeatU)*uv[0];
    float pixelYFloat = static_cast<float>(textureHeight*m_primitives[objectIndex]->material.textureMap.repeatV)*uv[1];
    int pixelX = static_cast<int>(pixelXFloat)%textureWidth;
    int pixelY = static_cast<int>(pixelYFloat)%textureHeight;

    glm::vec4 color = QrgbToVec4(m_primitiveTextures[objectIndex]->pixel(pixelX, pixelY));
    color = color * m_primitives[objectIndex]->material.blend;

    return color;
}

bool RayScene::shadowRay(glm::vec4 surfacePoint, glm::vec4 lightDirection, float lightDist){
    //Returns true if the provided light is occluded from the provided point
    float epsilon = 0.0001f;
    surfacePoint = surfacePoint + epsilon*lightDirection;
    glm::vec4 viewPlanePoint = surfacePoint + lightDirection;
    std::tuple<double, int> intersectionValues = getTValue(surfacePoint, viewPlanePoint);
    double t = std::get<0>(intersectionValues);
    double intDepth = glm::length(lightDirection) - epsilon;
    intDepth = intDepth * t;
    // The surface is in shadow if
    // 1) there is an object in the direction of the light AND
    // 2) the light is directional or the object lies in front of the point light
    if (intDepth > 0.0 && (lightDist < 0.0 || intDepth < lightDist)){
        return true;
    }
    return false;
}

glm::vec4 RayScene::rayLighting(glm::vec4 point, glm::vec4 viewPlanePoint, int recursionDepth) {
    //Returns the lighting value for the specified ray

    //check whether we've hit max recursion depth
    if (recursionDepth > m_maxrecursiondepth) {
        return glm::vec4(0.0);
    }

    std::tuple<double, int> intersectionValues = getTValue(point, viewPlanePoint);
    double t = std::get<0>(intersectionValues);
    int objectIndex = std::get<1>(intersectionValues);

    glm::vec4 channelColor = glm::vec4(0.0,0.0,0.0,1.0);
    if (t > 0.0) {
            channelColor = computeLighting(point, viewPlanePoint, t, objectIndex, recursionDepth);
    }
    return channelColor;
}

glm::vec4 RayScene::computeLighting(glm::vec4 point, glm::vec4 viewPlanePoint, double t, int objectIndex, int recursionDepth){
    /*
     * Computes the light value for a particular pixel
     * point          - the eye position (camera center) in world coords
     * viewPlanePoint - the point where our ray intersects the view plane in world coords
     * t              - the depth (t value) to the first intersection
     * The object that is intersected is stored by the member variable m_objectIndex
     */
    float epsilon = 0.0001f;


    //calculate the ambient component of the lighting
    glm::vec4 ambientTerm = m_primitives[objectIndex]->material.cAmbient * m_globalData->ka;

    glm::vec4 position = getPosition(point, viewPlanePoint, t);
    glm::vec4 normal = getNormal(position, objectIndex);
    normal = glm::vec4(glm::normalize(glm::vec3(normal)), 0.0);
    //unit vector going from object surface to eye
    glm::vec4 negLookVec = point - position;
    negLookVec = glm::vec4(glm::normalize(glm::vec3(negLookVec)), 0.0);
    //calculate the texture color from this view
    glm::vec4 textureColor = computeTextureColor(position, objectIndex);

    //calculate the diffuse component of the lighting
    glm::vec4 diffuseTerm = glm::vec4(0.0);
    glm::vec4 specularTerm = glm::vec4(0.0);
    for (int i = 0; i < static_cast<int>(m_lightData.size()); i++){
        glm::vec4 lightDirection = glm::vec4(0.0);
        float attenuationCoefficient = 1.0;
        //d is the distance to the point light
        float d = -1.f;
        if (m_lightData[i]->type == LightType::LIGHT_POINT){
            lightDirection = m_lightData[i]->pos - position;
            lightDirection = glm::vec4(glm::normalize(glm::vec3(lightDirection)), 0.0);
            //calculate the attenuation of the point light
            d = glm::length(m_lightData[i]->pos - position);
            attenuationCoefficient = 1.f / (m_lightData[i]->function.x + m_lightData[i]->function.y*d + m_lightData[i]->function.z*pow(d, 2.0));
        } else if (m_lightData[i]->type == LightType::LIGHT_DIRECTIONAL){
            lightDirection = glm::vec4(glm::normalize(glm::vec3(-1.f*m_lightData[i]->dir)), 0.0);
        } else {
            break;
        }

        //cast shadow ray for light
        bool inShadow = shadowRay(position, lightDirection, d);

        //no specular or diffuse contribution if the point is in shadow
        if (!inShadow){
            //compute the diffuse term for light i
            glm::vec4 diffuseTermLightI = (m_globalData->kd * m_primitives[objectIndex]->material.cDiffuse * (1.f - m_primitives[objectIndex]->material.blend) + textureColor) * attenuationCoefficient * m_lightData[i]->color * glm::max(glm::dot(normal, lightDirection), 0.f);
            //add the diffuse contribution of this light
            diffuseTerm = diffuseTerm + glm::max(diffuseTermLightI, glm::vec4(0.0,0.0,0.0,0.0));

            //get reflected light direction
            glm::vec4 reflectedLightDirection = reflectRay(lightDirection, normal);
            reflectedLightDirection = glm::vec4(glm::normalize(glm::vec3(reflectedLightDirection)), 0.0);
            //calculate the specular contribution of this light and add that
            glm::vec4 specularTermLightI =  m_lightData[i]->color * m_globalData->ks * m_primitives[objectIndex]->material.cSpecular * static_cast<float>(pow(glm::max(glm::dot(reflectedLightDirection, negLookVec), 0.f), m_primitives[objectIndex]->material.shininess));
            specularTerm = specularTerm + glm::max(specularTermLightI, glm::vec4(0.0,0.0,0.0,0.0));
            }
        }

    //recursively calculate the reflection
    glm::vec4 reflectedLook = reflectRay(negLookVec, normal);
    glm::vec4 reflectedEye = position + epsilon * reflectedLook;
    glm::vec4 reflectedViewPlanePoint = reflectedEye + reflectedLook;
    glm::vec4 recursiveIllumination = rayLighting(reflectedEye, reflectedViewPlanePoint, recursionDepth + 1);
    recursiveIllumination = recursiveIllumination * m_globalData->ks * m_primitives[objectIndex]->material.cReflective;


//    glm::vec4 lightDirection = m_lightData[0]->pos - position;
//    lightDirection = glm::vec4(glm::normalize(glm::vec3(lightDirection)), 0.0);
//    return (lightDirection + 1.0f)/2.f;;
//    return (normal+1.0f)/2.f;
    glm::vec4 totalLighting = ambientTerm + diffuseTerm + specularTerm + recursiveIllumination;
    totalLighting = glm::min(totalLighting, glm::vec4(1.0));
    return totalLighting;
}

