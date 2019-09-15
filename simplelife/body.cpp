#include "body.h"

Body::Body(Brain& parentBrain, variationType v_type, double ratio) : Brain(parentBrain, v_type, ratio)
{

}

Body::Body(string filePath): Brain(filePath)
{

}

Body::Body(uint64_t input_num, uint64_t neuralNode, uint64_t out_num, uint64_t _score) : Brain(input_num, neuralNode, out_num, _score)
{

}

Body::~Body()
{
}
