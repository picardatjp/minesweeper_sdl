#ifndef GATE_HPP
#define GATE_HPP
#include "SDL.h"

enum g_type
{
    G_NOT,
    G_AND,
    G_OR,
    G_XOR,
    G_NAND,
    G_NOR,
    G_XNOR
};

enum g_dir
{
    G_NORTH,
    G_SOUTH,
    G_WEST,
    G_EAST
};

class Gate
{
private:
    g_type gate_type_ = G_NOT;
    bool input_a_ = false;
    bool input_b_ = false;
    int iai_ = -1;
    int ibi_ = -1;
    bool output_;
    bool output_undefined_ = false;
    SDL_Rect dest_;
    g_dir dir_ = G_NORTH;

    void g_not() { output_ = !input_a_; }
    void g_and() { output_ = input_a_ && input_b_; }
    void g_or() { output_ = input_a_ || input_b_; }
    void g_xor() { output_ = input_a_ ^ input_b_; }
    void g_nand() { output_ = !(input_a_ && input_b_); }
    void g_nor() { output_ = !(input_a_ || input_b_); }
    void g_xnor() { output_ = !(input_a_ ^ input_b_); }

public:
    void setDir(g_dir d) { dir_ = d; }
    g_dir getDir() { return dir_; }
    void setType(g_type g) { gate_type_ = g; }
    void setA(bool a)
    {
        input_a_ = a;
        switch (gate_type_)
        {
        case G_NOT:
            g_not();
            break;
        case G_AND:
            g_and();
            break;
        case G_OR:
            g_or();
            break;
        case G_XOR:
            g_xor();
            break;
        case G_NAND:
            g_nand();
            break;
        case G_NOR:
            g_nor();
            break;
        case G_XNOR:
            g_xnor();
            break;
        default:
            break;
        }
    }
    void setB(bool b)
    {
        input_b_ = b;
        switch (gate_type_)
        {
        case G_NOT:
            g_not();
            break;
        case G_AND:
            g_and();
            break;
        case G_OR:
            g_or();
            break;
        case G_XOR:
            g_xor();
            break;
        case G_NAND:
            g_nand();
            break;
        case G_NOR:
            g_nor();
            break;
        case G_XNOR:
            g_xnor();
            break;
        default:
            break;
        }
    }
    bool getA() { return input_a_; }
    bool getB() { return input_b_; }
    bool getOutput() { return output_; }
    SDL_Rect *getRect() { return &dest_; }
    void setRect(SDL_Rect r) { dest_ = r; }
    SDL_Color getColor()
    {
        if (gate_type_ == G_NOT)
            return {255, 0, 255, 255};
        if (gate_type_ == G_AND)
            return {255, 255, 0, 255};
        if (gate_type_ == G_OR)
            return {0, 255, 255, 255};
        if (gate_type_ == G_XOR)
            return {0, 0, 255, 255};
        if (gate_type_ == G_NAND)
            return {0, 255, 0, 255};
        if (gate_type_ == G_NOR)
            return {255, 0, 0, 255};
        if (gate_type_ == G_XNOR)
            return {130, 130, 130, 255};
        return {255, 255, 255, 255};
    }
    void connectInputA(int a) { iai_ = a; }
    void connectInputB(int b) { ibi_ = b; }
    int getiai() { return iai_; }
    int getibi() { return ibi_; }
};

#endif