//------------------------------------------------------------------------------
// Font.cpp
//------------------------------------------------------------------------------

#include "Font.hpp"
#include <vector>

using namespace std;

namespace
{

    vector<quetzal::pdf::Font> s_fonts;

    quetzal::pdf::Font::metrics_type metrics_courier =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
        0,  600,  600,  600,  600,    0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,
        0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,  600,    0,  600,  600,  600,
      600,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,    0,  600,    0,    0,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0,
        0,  600,    0,    0,    0,  600,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_courier_bold =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
        0,  600,  600,  600,  600,    0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,
        0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,  600,    0,  600,  600,  600,
      600,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,    0,  600,    0,    0,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0,
        0,  600,    0,    0,    0,  600,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_courier_bold_oblique =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
        0,  600,  600,  600,  600,    0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,
        0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,  600,    0,  600,  600,  600,
      600,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,    0,  600,    0,    0,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0,
        0,  600,    0,    0,    0,  600,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_courier_oblique =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
      600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,  600,
        0,  600,  600,  600,  600,    0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,
        0,  600,  600,  600,  600,  600,  600,  600,  600,    0,  600,  600,    0,  600,  600,  600,
      600,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  600,    0,  600,    0,    0,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0,
        0,  600,    0,    0,    0,  600,    0,    0,  600,  600,  600,  600,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_helvetica =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      278,  278,  355,  556,  556,  889,  667,  222,  333,  333,  389,  584,  278,  333,  278,  278,
      556,  556,  556,  556,  556,  556,  556,  556,  556,  556,  278,  278,  584,  584,  584,  556,
     1015,  667,  667,  722,  722,  667,  611,  778,  722,  278,  500,  667,  556,  833,  722,  778,
      667,  778,  722,  667,  611,  722,  667,  944,  667,  667,  611,  278,  278,  278,  469,  556,
      222,  556,  556,  500,  556,  556,  278,  556,  556,  222,  222,  500,  222,  833,  556,  556,
      556,  556,  333,  500,  278,  556,  500,  722,  500,  500,  500,  334,  260,  334,  584,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  333,  556,  556,  167,  556,  556,  556,  556,  191,  333,  556,  333,  333,  500,  500,
        0,  556,  556,  556,  278,    0,  537,  350,  222,  333,  333,  556, 1000, 1000,    0,  611,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
     1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0, 1000,    0,  370,    0,    0,    0,    0,  556,  778, 1000,  365,    0,    0,    0,    0,
        0,  889,    0,    0,    0,  278,    0,    0,  222,  611,  944,  611,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_helvetica_bold =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      278,  333,  474,  556,  556,  889,  722,  278,  333,  333,  389,  584,  278,  333,  278,  278,
      556,  556,  556,  556,  556,  556,  556,  556,  556,  556,  333,  333,  584,  584,  584,  611,
      975,  722,  722,  722,  722,  667,  611,  778,  722,  278,  556,  722,  611,  833,  722,  778,
      667,  778,  722,  667,  611,  722,  667,  944,  667,  667,  611,  333,  278,  333,  584,  556,
      278,  556,  611,  556,  611,  556,  333,  611,  611,  278,  278,  556,  278,  889,  611,  611,
      611,  611,  389,  556,  333,  611,  556,  778,  556,  556,  500,  389,  280,  389,  584,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  333,  556,  556,  167,  556,  556,  556,  556,  238,  500,  556,  333,  333,  611,  611,
        0,  556,  556,  556,  278,    0,  556,  350,  278,  500,  500,  556, 1000, 1000,    0,  611,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
     1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0, 1000,    0,  370,    0,    0,    0,    0,  611,  778, 1000,  365,    0,    0,    0,    0,
        0,  889,    0,    0,    0,  278,    0,    0,  278,  611,  944,  611,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_helvetica_bold_oblique =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      278,  333,  474,  556,  556,  889,  722,  278,  333,  333,  389,  584,  278,  333,  278,  278,
      556,  556,  556,  556,  556,  556,  556,  556,  556,  556,  333,  333,  584,  584,  584,  611,
      975,  722,  722,  722,  722,  667,  611,  778,  722,  278,  556,  722,  611,  833,  722,  778,
      667,  778,  722,  667,  611,  722,  667,  944,  667,  667,  611,  333,  278,  333,  584,  556,
      278,  556,  611,  556,  611,  556,  333,  611,  611,  278,  278,  556,  278,  889,  611,  611,
      611,  611,  389,  556,  333,  611,  556,  778,  556,  556,  500,  389,  280,  389,  584,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  333,  556,  556,  167,  556,  556,  556,  556,  238,  500,  556,  333,  333,  611,  611,
        0,  556,  556,  556,  278,    0,  556,  350,  278,  500,  500,  556, 1000, 1000,    0,  611,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
     1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0, 1000,    0,  370,    0,    0,    0,    0,  611,  778, 1000,  365,    0,    0,    0,    0,
        0,  889,    0,    0,    0,  278,    0,    0,  278,  611,  944,  611,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_helvetica_oblique =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      278,  278,  355,  556,  556,  889,  667,  222,  333,  333,  389,  584,  278,  333,  278,  278,
      556,  556,  556,  556,  556,  556,  556,  556,  556,  556,  278,  278,  584,  584,  584,  556,
     1015,  667,  667,  722,  722,  667,  611,  778,  722,  278,  500,  667,  556,  833,  722,  778,
      667,  778,  722,  667,  611,  722,  667,  944,  667,  667,  611,  278,  278,  278,  469,  556,
      222,  556,  556,  500,  556,  556,  278,  556,  556,  222,  222,  500,  222,  833,  556,  556,
      556,  556,  333,  500,  278,  556,  500,  722,  500,  500,  500,  334,  260,  334,  584,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  333,  556,  556,  167,  556,  556,  556,  556,  191,  333,  556,  333,  333,  500,  500,
        0,  556,  556,  556,  278,    0,  537,  350,  222,  333,  333,  556, 1000, 1000,    0,  611,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
     1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0, 1000,    0,  370,    0,    0,    0,    0,  556,  778, 1000,  365,    0,    0,    0,    0,
        0,  889,    0,    0,    0,  278,    0,    0,  222,  611,  944,  611,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_symbol =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      250,  333,  713,  500,  549,  833,  778,  439,  333,  333,  500,  549,  250,  549,  250,  278,
      500,  500,  500,  500,  500,  500,  500,  500,  500,  500,  278,  278,  549,  549,  549,  444,
      549,  722,  667,  722,  612,  611,  763,  603,  722,  333,  631,  722,  686,  889,  722,  722,
      768,  741,  556,  592,  611,  690,  439,  768,  645,  795,  611,  333,  863,  333,  658,  500,
      500,  631,  549,  549,  494,  439,  521,  411,  603,  329,  603,  549,  549,  576,  521,  549,
      549,  521,  549,  603,  439,  576,  713,  686,  493,  686,  494,  480,  200,  480,  549,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      750,  620,  247,  549,  167,  713,  500,  753,  753,  753,  753, 1042,  987,  603,  987,  603,
      400,  549,  411,  549,  549,  713,  494,  460,  549,  549,  549,  549, 1000,  603, 1000,  658,
      823,  686,  795,  987,  768,  768,  823,  768,  768,  713,  713,  713,  713,  713,  713,  713,
      768,  713,  790,  790,  890,  823,  549,  250,  713,  603,  603, 1042,  987,  603,  987,  603,
      494,  329,  790,  790,  786,  713,  384,  384,  384,  384,  384,  384,  494,  494,  494,  494,
        0,  329,  274,  686,  686,  686,  384,  384,  384,  384,  384,  384,  494,  494,  494,    0
    };

    quetzal::pdf::Font::metrics_type metrics_times_bold =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      250,  333,  555,  500,  500, 1000,  833,  333,  333,  333,  500,  570,  250,  333,  250,  278,
      500,  500,  500,  500,  500,  500,  500,  500,  500,  500,  333,  333,  570,  570,  570,  500,
      930,  722,  667,  722,  722,  667,  611,  778,  778,  389,  500,  778,  667,  944,  722,  778,
      611,  778,  722,  556,  667,  722,  722, 1000,  722,  722,  667,  333,  278,  333,  581,  500,
      333,  500,  556,  444,  556,  444,  333,  500,  556,  278,  333,  556,  278,  833,  556,  500,
      556,  556,  444,  389,  333,  556,  500,  722,  500,  500,  444,  394,  220,  394,  520,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  333,  500,  500,  167,  500,  500,  500,  500,  278,  500,  500,  333,  333,  556,  556,
        0,  500,  500,  500,  250,    0,  540,  350,  333,  500,  500,  500, 1000, 1000,    0,  500,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
     1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0, 1000,    0,  300,    0,    0,    0,    0,  667,  778, 1000,  330,    0,    0,    0,    0,
        0,  722,    0,    0,    0,  278,    0,    0,  278,  500,  722,  556,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_times_bold_italic =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      250,  389,  555,  500,  500,  833,  778,  333,  333,  333,  500,  570,  250,  333,  250,  278,
      500,  500,  500,  500,  500,  500,  500,  500,  500,  500,  333,  333,  570,  570,  570,  500,
      832,  667,  667,  667,  722,  667,  667,  722,  778,  389,  500,  667,  611,  889,  722,  722,
      611,  722,  667,  556,  611,  722,  667,  889,  667,  611,  611,  333,  278,  333,  570,  500,
      333,  500,  500,  444,  500,  444,  333,  500,  556,  278,  278,  500,  278,  778,  556,  500,
      500,  500,  389,  389,  278,  556,  444,  667,  500,  444,  389,  348,  220,  348,  570,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  389,  500,  500,  167,  500,  500,  500,  500,  278,  500,  500,  333,  333,  556,  556,
        0,  500,  500,  500,  250,    0,  500,  350,  333,  500,  500,  500, 1000, 1000,    0,  500,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
     1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  944,    0,  266,    0,    0,    0,    0,  611,  722,  944,  300,    0,    0,    0,    0,
        0,  722,    0,    0,    0,  278,    0,    0,  278,  500,  722,  500,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_times_italic =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      250,  333,  420,  500,  500,  833,  778,  333,  333,  333,  500,  675,  250,  333,  250,  278,
      500,  500,  500,  500,  500,  500,  500,  500,  500,  500,  333,  333,  675,  675,  675,  500,
      920,  611,  611,  667,  722,  611,  611,  722,  722,  333,  444,  667,  556,  833,  667,  722,
      611,  722,  611,  500,  556,  722,  611,  833,  611,  556,  556,  389,  278,  389,  422,  500,
      333,  500,  500,  444,  500,  444,  278,  500,  500,  278,  278,  444,  278,  722,  500,  500,
      500,  500,  389,  389,  278,  500,  444,  667,  444,  444,  389,  400,  275,  400,  541,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  389,  500,  500,  167,  500,  500,  500,  500,  214,  556,  500,  333,  333,  500,  500,
        0,  500,  500,  500,  250,    0,  523,  350,  333,  556,  556,  500,  889, 1000,    0,  500,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
      889,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  889,    0,  276,    0,    0,    0,    0,  556,  722,  944,  310,    0,    0,    0,    0,
        0,  667,    0,    0,    0,  278,    0,    0,  278,  500,  667,  500,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_times_roman =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      250,  333,  408,  500,  500,  833,  778,  333,  333,  333,  500,  564,  250,  333,  250,  278,
      500,  500,  500,  500,  500,  500,  500,  500,  500,  500,  278,  278,  564,  564,  564,  444,
      921,  722,  667,  667,  722,  611,  556,  722,  722,  333,  389,  722,  611,  889,  722,  722,
      556,  722,  667,  556,  611,  722,  722,  944,  722,  722,  611,  333,  278,  333,  469,  500,
      333,  444,  500,  444,  500,  444,  333,  500,  500,  278,  278,  500,  278,  778,  500,  500,
      500,  500,  333,  389,  278,  500,  500,  722,  500,  500,  444,  480,  200,  480,  541,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  333,  500,  500,  167,  500,  500,  500,  500,  180,  444,  500,  333,  333,  556,  556,
        0,  500,  500,  500,  250,    0,  453,  350,  333,  444,  444,  500, 1000, 1000,    0,  444,
        0,  333,  333,  333,  333,  333,  333,  333,  333,    0,  333,  333,    0,  333,  333,  333,
     1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  889,    0,  276,    0,    0,    0,    0,  611,  722,  889,  310,    0,    0,    0,    0,
        0,  667,    0,    0,    0,  278,    0,    0,  278,  500,  722,  500,    0,    0,    0,    0
    };

    quetzal::pdf::Font::metrics_type metrics_zapf_dingbats =
    {
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      278,  974,  961,  974,  980,  719,  789,  790,  791,  690,  960,  939,  549,  855,  911,  933,
      911,  945,  974,  755,  846,  762,  761,  571,  677,  763,  760,  759,  754,  494,  552,  537,
      577,  692,  786,  788,  788,  790,  793,  794,  816,  823,  789,  841,  823,  833,  816,  831,
      923,  744,  723,  749,  790,  792,  695,  776,  768,  792,  759,  707,  708,  682,  701,  826,
      815,  789,  789,  707,  687,  696,  689,  786,  787,  713,  791,  785,  791,  873,  761,  762,
      762,  759,  759,  892,  892,  788,  784,  438,  138,  277,  415,  392,  392,  668,  668,    0,
      390,  390,  317,  317,  276,  276,  509,  509,  410,  410,  234,  234,  334,  334,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  732,  544,  544,  910,  667,  760,  760,  776,  595,  694,  626,  788,  788,  788,  788,
      788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,
      788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,  788,
      788,  788,  788,  788,  894,  838, 1016,  458,  748,  924,  748,  918,  927,  928,  928,  834,
      873,  828,  924,  924,  917,  930,  931,  463,  883,  836,  836,  867,  867,  696,  696,  874,
        0,  874,  760,  946,  771,  865,  771,  888,  967,  888,  831,  873,  927,  970,  918,    0
    };

} // namespace

//------------------------------------------------------------------------------
quetzal::pdf::Font::Font() :
    m_id(0),
    m_name(),
    m_metrics()
{
}

//------------------------------------------------------------------------------
quetzal::pdf::Font::Font(id_type id, const string& name, const metrics_type& metrics) :
    m_id(id),
    m_name(name),
    m_metrics(metrics)
{
}

//------------------------------------------------------------------------------
quetzal::pdf::id_type quetzal::pdf::Font::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
string quetzal::pdf::Font:: name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
double quetzal::pdf::Font::width(double size, const string& s) const
{
    double width = 0.;

    for (const auto& i : s)
    {
         width += size * m_metrics[i] / 1000.;
    }

    return width;
}

//------------------------------------------------------------------------------
void quetzal::pdf::init_fonts()
{
    s_fonts =
    {
        {1, "Courier", metrics_courier},
        {2, "Courier-Bold", metrics_courier_bold},
        {3, "Courier-Oblique", metrics_courier_bold_oblique},
        {4, "Courier-BoldOblique", metrics_courier_oblique},
        {5, "Helvetica", metrics_helvetica},
        {6, "Helvetica-Bold", metrics_helvetica_bold},
        {7, "Helvetica-Oblique", metrics_helvetica_bold_oblique},
        {8, "Helvetica-BoldOblique", metrics_helvetica_oblique},
        {9, "Symbol", metrics_symbol},
        {10, "Times-Roman", metrics_times_bold},
        {11, "Times-Bold", metrics_times_bold_italic},
        {12, "Times-Italic", metrics_times_italic},
        {13, "Times-BoldItalic", metrics_times_roman},
        {14, "ZapfDingbats", metrics_zapf_dingbats}
    };

    return;
}

//------------------------------------------------------------------------------
unsigned int quetzal::pdf::fonts()
{
    return s_fonts.size();
}

//------------------------------------------------------------------------------
const quetzal::pdf::Font* quetzal::pdf::font(id_type id)
{
    if (id >= s_fonts.size())
    {
        return nullptr;
    }

    return &s_fonts[id];
}

//------------------------------------------------------------------------------
const quetzal::pdf::Font* quetzal::pdf::font(const string& name)
{
    for (const auto& font : s_fonts)
    {
        if (name == font.name())
        {
            return &font;
        }
    }

    return nullptr;
}

//------------------------------------------------------------------------------
vector<string> quetzal::pdf::word_wrap(const Font& font, double size, double wMax, const string& s, bool bRightJustify)
{
    // First, break s up into separate words

    vector<string> words;
    string word;

    for (int i = 0, n = s.size(); i < n; ++i)
    {
        char c = s[i];

        if (!word.empty())
        {
             if (isspace(c))
             {
                 words.push_back(word);
                 word.clear();
             }
             else
             {
                 word += c;
             }
        }
        else
        {
             if (!isspace(c))
             {
                 word += c;
             }
        }
    }

    if (!word.empty())
    {
        words.push_back(word);
    }

#if 0
    cout << "Initial: [" << s << "]" << endl;

    for (int i = 0; i < (int)words.size(); ++i)
    {
        cout << "[" << words[i] << "]" << endl;
    }

#endif

    vector<string> result;
    string currentLine;

    for (int i = 0, n = words.size(); i < n; ++i)
    {
        string ss = currentLine;

        if (!currentLine.empty())
        {
            switch (currentLine[currentLine.size() - 1])
            {
                case '.':
                case '?':
                case '!':
                    ss += " ";
                    break;
            }

            ss += " ";
        }

        ss += words[i];

        if (font.width(size, ss) > wMax)
        {
            result.push_back(currentLine);
            currentLine = words[i];
        }
        else
        {
            currentLine = ss;
        }
    }

    result.push_back(currentLine);

    // If so requested, right justify each line

    if (bRightJustify)
    {
        string aSpace = " ";
        double spaceWidth = font.width(size, aSpace);

        // Right justify each line by interspersing additional spaces between the words until adding an additional space would make the line too long.

        for (int i = 0, n = result.size(); i < n; ++i)
        {
             string line = result[i];

             if (font.width(size, line) + spaceWidth <= wMax)
             {
                 // Store off our indices, the locations in 'line' of the first space after each word:
                 //
                 //   here  is    a string    of some  words
                 //       ^   ^    ^      ^     ^    ^

                 vector<int> indices;

                 for (int j = 0, nl = line.size(); j < nl; ++j)
                 {
                     if (j > 0 && !isspace(line[j - 1]) && isspace(line[j]))
                     {
                         indices.push_back(j);
                     }
                 }

                 int nIndices = indices.size();

                 // We only add spaces between words so unless there are 2+ words there's nothing to do

                 if (nIndices > 0)
                 {
                     // We first insert a space at indices[0], then at indices[1], etc.
                     // The variable 'theIndex' indices which offset in 'indices' we're using

                     int theIndex = 0;

                     while (font.width(size, line) + spaceWidth <= wMax)
                     {
                         line = line.substr(0, indices[theIndex]) + aSpace + line.substr(indices[theIndex]);

                         // Because we added a space to the string after indices[theIndex], we need to add one to all our offsets after 'theIndex' to reflect the addition of the space
                         for (int j = theIndex + 1; j < nIndices; ++j)
                         {
                             indices[j]++;
                         }

                         // If we've hit our last two words, roll back around to the first two words to add the next space
                         if (++theIndex == nIndices)
                         {
                             theIndex = 0;
                         }
                     }
                 }

                 // Update the entry in 'result'
                 result[i] = line;
            }
        }
    }

    return result;
}
