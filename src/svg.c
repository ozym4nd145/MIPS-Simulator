#include "svg.h"
#include <stdio.h>
#include "global_vars.h"

void start_svg(FILE* ptr)
{
  fprintf(
      ptr,
      "<svg   xmlns:dc=\"http://purl.org/dc/elements/1.1/\"   "
      "xmlns:cc=\"http://creativecommons.org/ns#\"   "
      "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"   "
      "xmlns:svg=\"http://www.w3.org/2000/svg\"   "
      "xmlns=\"http://www.w3.org/2000/svg\"   "
      "xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/"
      "sodipodi-0.dtd\"   "
      "xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"   "
      "width=\"%fmm\"   height=\"%fmm\"   viewBox=\"0 0 297 210\"   "
      "version=\"1.1\"   id=\"SVGRoot\"   inkscape:version=\"0.92.1 "
      "unknown\" onload= \"refresh()\" >\n"
      "<filter id=\"colorsaturate\">    <feColorMatrix "
      "in=\"SourceGraphic\" type=\"saturate\" values=\"%f\" />  </filter>\n",
      SVG_WIDTH, SVG_HEIGHT, SVG_SATURATE);
}

void end_svg(FILE* ptr)
{
  fprintf(ptr,
          "\n<script type=\"text/javascript\"> function refresh() { "
          "setTimeout(function() {location.reload(true)},500); } "
          "</script>\n</svg>\n");
}
void draw_fetch(FILE* ptr, int active)
{
  char* header =
      "<g     id=\"layer1\"     inkscape:groupmode=\"layer\"     "
      "inkscape:label=\"Fetch\"     style=\"display:inline\">    <g       "
      "id=\"g6881\"       transform=\"matrix(1.1599742,0,0,1,-2.7539454,0)\"   "
      "  "
      "  inkscape:label=\"Fetch\"";
  char* body =
      ">      <g         id=\"g6969\"         "
      "transform=\"translate(-3.9353519,-6.967489)\">        <rect           "
      "style=\"fill:#18c0ee;fill-opacity:1;fill-rule:evenodd;stroke:#020202;"
      "stroke-width:0.35509527;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "miterlimit:4;stroke-dasharray:none;stroke-opacity:1\"           "
      "id=\"rect6650\"           width=\"2.2147138\"           "
      "height=\"86.657448\"           x=\"68.983871\"           "
      "y=\"67.444626\" "
      "/>        <g           "
      "transform=\"matrix(1,0,0,0.90336385,-0.14645863,13.773331)\"           "
      "id=\"g6837\">          <g             id=\"g6795\">            <path    "
      "  "
      "         inkscape:connector-curvature=\"0\"               "
      "style=\"display:inline;overflow:visible;visibility:visible;opacity:1;"
      "fill:"
      "#15bdeb;fill-opacity:1;stroke:#000000;stroke-width:0.37894216;stroke-"
      "linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:"
      "none;stroke-dashoffset:0;stroke-opacity:1;marker:none;enable-background:"
      "accumulate\"               d=\"m 22.047507,121.15217 v 10.74881 l "
      "3.339839,-2.14976 v -6.44929 z\"               id=\"path2627\"          "
      "  "
      "   sodipodi:nodetypes=\"ccccc\" />            <text               "
      "xml:space=\"preserve\"               "
      "style=\"font-style:normal;font-variant:normal;font-weight:normal;font-"
      "stretch:normal;font-size:2.27365279px;line-height:0%;font-family:'"
      "DejaVu "
      "Sans';-inkscape-font-specification:'DejaVu "
      "Sans';text-align:center;writing-mode:lr-tb;text-anchor:middle;fill:#"
      "000000;fill-opacity:1;stroke-width:0.18947108\"               "
      "x=\"111.45602\"               y=\"-26.104834\"               "
      "id=\"text2629\"               "
      "transform=\"matrix(0,1.1346124,-0.88135823,0,0,0)\"><tspan              "
      "  "
      " style=\"font-size:2.27365279px;line-height:1;stroke-width:0.18947108\" "
      "  "
      "              sodipodi:role=\"line\"                 id=\"tspan2631\"   "
      "  "
      "            x=\"111.45602\"                 "
      "y=\"-26.104834\">MUX</tspan></text>            <path               "
      "style=\"display:inline;overflow:visible;visibility:visible;opacity:1;"
      "fill:"
      "#15bdeb;fill-opacity:1;stroke:#000000;stroke-width:0.41162226;stroke-"
      "linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;stroke-dasharray:"
      "none;stroke-dashoffset:0;stroke-opacity:1;marker:none;enable-background:"
      "accumulate\"               d=\"m 52.715445,78.5237 4.92529,3.870065 v "
      "10.320173 l -4.92529,3.870064 v -7.740129 l 1.641767,-1.290022 "
      "-1.641767,-1.290021 z\"               id=\"path2402\"               "
      "sodipodi:nodetypes=\"cccccccc\"               "
      "inkscape:connector-curvature=\"0\" />            <rect               "
      "style=\"fill:#15bdeb;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.35509527;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "miterlimit:4;stroke-dasharray:none;stroke-opacity:1\"               "
      "id=\"rect5376\"               width=\"4.4533725\"               "
      "height=\"30.938215\"               x=\"31.939384\"               "
      "y=\"111.05746\" />            <text               "
      "xml:space=\"preserve\"  "
      "             "
      "style=\"font-style:normal;font-weight:normal;font-size:3.40941286px;"
      "line-"
      "height:125%;font-family:Sans;letter-spacing:0px;word-spacing:0px;fill:#"
      "000000;fill-opacity:1;stroke:none;stroke-width:0.08523532px;stroke-"
      "linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"               "
      "x=\"44.061661\"               y=\"93.773392\"               "
      "id=\"text6464\"               "
      "transform=\"scale(0.73596426,1.3587616)\"><tspan                 "
      "sodipodi:role=\"line\"                 id=\"tspan6462\"                 "
      "x=\"44.061661\"                 y=\"93.773392\"                 "
      "style=\"stroke-width:0.08523532px\">PC</tspan></text>            <rect  "
      "  "
      "           "
      "style=\"fill:#18c0ee;fill-opacity:1;fill-rule:evenodd;stroke:#020202;"
      "stroke-width:0.44490373;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "miterlimit:4;stroke-dasharray:none;stroke-opacity:1\"               "
      "id=\"rect5378\"               width=\"15.145014\"               "
      "height=\"31.452288\"               x=\"47.689545\"               "
      "y=\"110.80043\" />            <text               "
      "xml:space=\"preserve\"  "
      "             "
      "style=\"font-style:normal;font-weight:normal;font-size:3.18622828px;"
      "line-"
      "height:125%;font-family:Sans;text-align:center;letter-spacing:0px;word-"
      "spacing:0px;text-anchor:middle;fill:#000000;fill-opacity:1;stroke:none;"
      "stroke-width:0.07965569px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-opacity:1\"               x=\"66.775391\"               "
      "y=\"102.74715\"               id=\"text6468\"               "
      "transform=\"scale(0.82967026,1.2052981)\"><tspan                 "
      "sodipodi:role=\"line\"                 id=\"tspan6466\"                 "
      "x=\"66.775391\"                 y=\"102.74715\"                 "
      "style=\"text-align:center;text-anchor:middle;stroke-width:0."
      "07965569px\">"
      "Instruction</tspan><tspan                 sodipodi:role=\"line\"        "
      "  "
      "       x=\"66.775391\"                 y=\"106.85062\"                 "
      "style=\"text-align:center;text-anchor:middle;stroke-width:0."
      "07965569px\"  "
      "               id=\"tspan6470\">Memory</tspan></text>            <path  "
      "  "
      "           "
      "style=\"fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0."
      "35509527;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;"
      "stroke-dasharray:none;stroke-opacity:1\"               d=\"M "
      "25.480312,126.52657 H 31.82759\"               id=\"path6500\"          "
      "  "
      "   inkscape:connector-curvature=\"0\" />            <path               "
      "style=\"fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0."
      "35509527;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;"
      "stroke-dasharray:none;stroke-opacity:1\"               d=\"M "
      "36.326855,126.52657 H 47.44961\"               id=\"path6500-3\"        "
      "  "
      "     inkscape:connector-curvature=\"0\" />            <path             "
      "  "
      "style=\"fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0."
      "35509527;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;"
      "stroke-dasharray:none;stroke-opacity:1\"               d=\"M "
      "41.347168,126.72494 V 93.497986 l 11.265648,-0.303442\"               "
      "id=\"path6532\"               inkscape:connector-curvature=\"0\" />     "
      "  "
      "     <path               "
      "style=\"fill:#020d10;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.1977087px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               d=\"m 51.456397,92.476484 0.03972,1.623767 "
      "1.111601,-0.91337 z\"               id=\"path6534\"               "
      "inkscape:connector-curvature=\"0\" />            <path               "
      "style=\"fill:#020d10;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.1977087px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               d=\"m 46.29829,125.70637 0.03972,1.62377 "
      "1.1116,-0.91336 z\"               id=\"path6534-7\"               "
      "inkscape:connector-curvature=\"0\" />            <path               "
      "style=\"fill:#020d10;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.1977087px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               d=\"m 30.676269,125.70637 0.03972,1.62377 "
      "1.1116,-0.91336 z\"               id=\"path6534-5\"               "
      "inkscape:connector-curvature=\"0\" />            <ellipse               "
      "style=\"fill:#0b0b0b;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.0463914px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               id=\"path6593\"               "
      "cx=\"41.253933\"  "
      "             cy=\"126.35329\"               rx=\"0.4108012\"            "
      "  "
      " ry=\"0.62422723\" />            <path               "
      "style=\"fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0."
      "35509527;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;"
      "stroke-dasharray:none;stroke-opacity:1\"               d=\"M "
      "41.467326,82.47844 H 52.590079\"               id=\"path6500-3-3\"      "
      "  "
      "       inkscape:connector-curvature=\"0\" />            <path           "
      "  "
      "  "
      "style=\"fill:#020d10;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.1977087px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               d=\"m 51.438759,81.76804 0.03972,1.623768 "
      "1.1116,-0.913368 z\"               id=\"path6534-7-5\"               "
      "inkscape:connector-curvature=\"0\" />            <text               "
      "xml:space=\"preserve\"               "
      "style=\"font-style:normal;font-weight:normal;font-size:3.75809121px;"
      "line-"
      "height:125%;font-family:Sans;letter-spacing:0px;word-spacing:0px;fill:#"
      "000000;fill-opacity:1;stroke:none;stroke-width:0.09395228px;stroke-"
      "linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"               "
      "x=\"48.802399\"               y=\"67.994934\"               "
      "id=\"text6627\"               "
      "transform=\"scale(0.81123088,1.2326947)\"><tspan                 "
      "sodipodi:role=\"line\"                 id=\"tspan6625\"                 "
      "x=\"48.802399\"                 y=\"67.994934\"                 "
      "style=\"stroke-width:0.09395228px\">4</tspan></text>            <path   "
      "  "
      "          "
      "style=\"fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0."
      "35509527;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;"
      "stroke-dasharray:none;stroke-opacity:1\"               d=\"m "
      "57.771882,87.636653 6.228883,0.210335 -0.138418,-13.882019 "
      "-46.508991,0.631 -0.13842,49.218061 h 4.567847\"               "
      "id=\"path6629\"               inkscape:connector-curvature=\"0\" />     "
      "  "
      "     <path               "
      "style=\"fill:#020d10;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.1977087px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               d=\"m 20.631463,123.10363 0.03972,1.62377 "
      "1.1116,-0.91337 z\"               id=\"path6534-6\"               "
      "inkscape:connector-curvature=\"0\" />            <path               "
      "style=\"fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0."
      "25939471;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;"
      "stroke-dasharray:none;stroke-opacity:1\"               d=\"m "
      "63.864977,80.90347 4.977845,0.215351\"               id=\"path6652\"    "
      "  "
      "         inkscape:connector-curvature=\"0\" />            <path         "
      "  "
      "    "
      "style=\"fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:0."
      "27143195;stroke-linecap:butt;stroke-linejoin:miter;stroke-miterlimit:4;"
      "stroke-dasharray:none;stroke-opacity:1\"               d=\"m "
      "62.895774,126.54628 5.947313,0.21452\"               id=\"path6654\"    "
      "  "
      "         inkscape:connector-curvature=\"0\" />            <path         "
      "  "
      "    "
      "style=\"fill:#020d10;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.1977087px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               d=\"m 67.691766,126.0504 0.03972,1.62377 "
      "1.1116,-0.91337 z\"               id=\"path6534-7-2\"               "
      "inkscape:connector-curvature=\"0\" />            <path               "
      "style=\"fill:#020d10;fill-opacity:1;fill-rule:evenodd;stroke:#000000;"
      "stroke-width:0.1977087px;stroke-linecap:butt;stroke-linejoin:miter;"
      "stroke-"
      "opacity:1\"               d=\"m 67.691501,80.40842 0.03972,1.623768 "
      "1.1116,-0.913367 z\"               id=\"path6534-7-9\"               "
      "inkscape:connector-curvature=\"0\" />            <text               "
      "xml:space=\"preserve\"               "
      "style=\"font-style:normal;font-weight:normal;font-size:2.56025362px;"
      "line-"
      "height:125%;font-family:Sans;letter-spacing:0px;word-spacing:0px;fill:#"
      "000000;fill-opacity:1;stroke:none;stroke-width:0.38403803px;stroke-"
      "linecap:butt;stroke-linejoin:miter;stroke-opacity:1\"               "
      "x=\"90.727188\"               y=\"-48.944641\"               "
      "id=\"text7092\"               "
      "transform=\"matrix(0.02873029,0.93739442,-1.0660125,0.02526389,0,0)\"><"
      "tspan                 sodipodi:role=\"line\"                 "
      "id=\"tspan7090\"                 x=\"90.727188\"                 "
      "y=\"-48.944641\"                 "
      "style=\"font-size:2.56025362px;stroke-width:0.38403803px\">Adder</"
      "tspan></"
      "text>          </g>        </g>      </g>    </g>  </g>";

  if (!active)
  {
    fprintf(ptr, "%s filter=\"url(#colorsaturate)\" %s ", header, body);
  }
  else
  {
    fprintf(ptr, "%s %s", header, body);
  }
}
void draw_decode(FILE* ptr, int active)
{
  char* header = "";
  char* body = "";
  if (!active)
  {
    fprintf(ptr, "%s filter=\"url(#colorsaturate)\" %s ", header, body);
  }
  else
  {
    fprintf(ptr, "%s %s", header, body);
  }
}
void draw_alu(FILE* ptr, int active)
{
  char* header = "";
  char* body = "";
  if (!active)
  {
    fprintf(ptr, "%s filter=\"url(#colorsaturate)\" %s ", header, body);
  }
  else
  {
    fprintf(ptr, "%s %s", header, body);
  }
}
void draw_memory(FILE* ptr, int active)
{
  char* header = "";
  char* body = "";
  if (!active)
  {
    fprintf(ptr, "%s filter=\"url(#colorsaturate)\" %s ", header, body);
  }
  else
  {
    fprintf(ptr, "%s %s", header, body);
  }
}
void draw_write(FILE* ptr, int active)
{
  char* header = "";
  char* body = "";
  if (!active)
  {
    fprintf(ptr, "%s filter=\"url(#colorsaturate)\" %s ", header, body);
  }
  else
  {
    fprintf(ptr, "%s %s", header, body);
  }
}