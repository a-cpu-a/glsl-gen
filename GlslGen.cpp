// GlslGen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <string>
#include <fstream>


//settings
const std::string uvCoordVariable = "(fragCoord/iResolution.xy)";//shadertoy
const std::string timeVariable = "iTime";//shadertoy
const std::string colorOutputVarible = "fragColor";//shadertoy
const std::string mainFunction = "void mainImage( out vec4 fragColor, in vec2 fragCoord )";//shadertoy
const bool hasVersion = false;
const bool hasInputsOutput = false;
const float maxConstFloat = 1.0f;
const size_t varibleCount = 3;
const size_t maxInstructionCount = 40;
//
std::random_device dev;
std::mt19937 rng(dev());

std::uniform_real_distribution<float> f(0.0f,1.0f);

struct OperatorData
{
    uint32_t id;
    std::string name;
};


float randFloat()
{
    return (f(rng) * maxConstFloat+0.01f)* 0.99009901f;
}

uint32_t randInt(uint32_t min, uint32_t max)
{
    std::uniform_int_distribution<uint32_t> i(min,max);
    return i(rng);
}

std::vector<std::string> varibles = { {"<RNG>","<RNG>","<UV>","<TIME>"}};

std::string getVarible(std::string swizzle)
{
    const std::string o = varibles[randInt(0,varibles.size()-1)];
    if (o == "<RNG>")
        return std::to_string(randFloat());
    else if (o == "<UV>")
    {
        if (randInt(0, 1) == 0)
            return uvCoordVariable + ".x";
        else
            return uvCoordVariable + ".y";
    }
    else if (o == "<TIME>")
        return timeVariable;
    return o + "." + swizzle;

}

const std::vector<OperatorData> operators = { {{0,"/"},{0,"*"},{0,"+"},{0,"-"},{0,"/"},{0,"*"},{0,"+"},{0,"-"},{1,"abs"},{1,"log2"},{1,"sin"},{1,"cos"},{1,"exp2"},{1,"inversesqrt"}} };
OperatorData getInstruction()
{
    OperatorData r = operators[randInt(0, operators.size() - 1)];
    return r;
}

int main()
{

    std::string out;
    if (hasVersion)
        out += "#version 420\n";
    if (hasInputsOutput)
    {
        out += "out vec4 " + colorOutputVarible + ";";
        out += "in vec2 " + uvCoordVariable + ";\n";
    }
    out += mainFunction + "\n{\n";

    for (size_t i = 0; i < varibleCount; i++)
    {
        std::string varibleName = ">ERROR<";
        if (i == varibleCount - 1)
        {
            varibleName = colorOutputVarible;
            out += "\t" + varibleName + " = vec4(" + getVarible("r") + "," + getVarible("g") + "," + getVarible("b") + ", 1);\n";
        }
        else
        {
            varibleName = "_v" + std::to_string(i);
            out += "\tvec4 " + varibleName + " = vec4(" + getVarible("r") + "," + getVarible("g") + "," + getVarible("b") + ", 1);\n";
        }

        for (size_t i = 0; i < randInt(1, maxInstructionCount); i++)
        {
            OperatorData opData = getInstruction();
            if(opData.id == 1 && randInt(0,1)==1)
                opData = getInstruction();

            if (opData.id ==1)//is a function
            {
                out += "\t" + varibleName + " = "+ opData.name +"(" + varibleName + ");\n";
            }
            else
            {
                out += "\t" + varibleName + " "+ opData.name  +"= vec4(" + getVarible("r") + "," + getVarible("g") + "," + getVarible("b") + ", 0);\n";
                //varibles.push_back(varibleName);
                //varibles.push_back("^");
            }


        }

        varibles.push_back(varibleName);
    }

    if (true)
    {
        out += "\n\n\t"+colorOutputVarible+" *= (iMouse.x*0.7)/iMouse.y;\n";
        out += "\t" + colorOutputVarible + " = abs(" + colorOutputVarible + ");\n";
    }

    out += "}\n";

    std::ofstream file("out.glsl");
    file << out;
    file.close();

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
