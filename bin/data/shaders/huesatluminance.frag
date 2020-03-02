#version 140
//#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex;

uniform float hue;
uniform float sat;
uniform float luminance;
uniform float tintR;
uniform float tintG;
uniform float tintB;

// Max, Min Functions.
float maxCom(vec4 col)
{
    return max(col.r, max(col.g,col.b));
}

float minCom(vec4 col)
{
    return min(col.r, min(col.g,col.b));
}
/*
 * Returns a vec4 with components h,s,l,a.
 */
vec4 rgbToHsl(vec4 col)
{
    float maxComponent = maxCom(col);
    float minComponent = minCom(col);
    float dif = maxComponent - minComponent;
    float add = maxComponent + minComponent;
    vec4 outColor = vec4(0.0, 0.0, 0.0, col.a);
    
    if (minComponent == maxComponent)
    {
        outColor.r = 0.0;
    }
    else if (col.r == maxComponent)
    {
        outColor.r = mod(((60.0 * (col.g - col.b) / dif) + 360.0), 360.0);
    }
    else if (col.g == maxComponent)
    {
        outColor.r = (60.0 * (col.b - col.r) / dif) + 120.0;
    }
    else
    {
        outColor.r = (60.0 * (col.r - col.g) / dif) + 240.0;
    }

    outColor.b = 0.5 * add;
    
    if (outColor.b == 0.0)
    {
        outColor.g = 0.0;
    }
    else if (outColor.b <= 0.5)
    {
        outColor.g = dif / add;
    }
    else
    {
        outColor.g = dif / (2.0 - add);
    }
    
    outColor.r /= 360.0;
    
    return outColor;
}
/*
 * Returns a component based on luminanceinocity p, saturation q, and hue h. 
 */
float hueToRgb(float p, float q, float h)
{
    if (h < 0.0)
    {
        h += 1.0;
    }
    else if (h > 1.0)
    {
        h -= 1.0;
    }
    if ((h * 6.0) < 1.0)
    {
        return p + (q - p) * h * 6.0;
    }
    else if ((h * 2.0) < 1.0)
    {
        return q;
    }
    else if ((h * 3.0) < 2.0)
    {
        return p + (q - p) * ((2.0 / 3.0) - h) * 6.0;
    }
    else
    {
        return p;
    }
}
/*
 * Returns a vec4 with components r,g,b,a, based off vec4 col with components h,s,l,a.
 */
vec4 hslToRgb(vec4 col)
{
    vec4 outColor = vec4(0.0, 0.0, 0.0, col.a);
    float p, q, tr, tg, tb;
    if (col.b <= 0.5)
    {
        q = col.b * (1.0 + col.g);
    }
    else
    {
        q = col.b + col.g - (col.b * col.g);
    }

    p = 2.0 * col.b - q;
    tr = col.r + (1.0 / 3.0);
    tg = col.r;
    tb = col.r - (1.0 / 3.0);

    outColor.r = hueToRgb(p, q, tr);
    outColor.g = hueToRgb(p, q, tg);
    outColor.b = hueToRgb(p, q, tb);

    return outColor;
}

void main( void )

{
	vec4 texcol = texture2DRect(tex,gl_TexCoord[0].st);
    vec4 hsl = rgbToHsl(texcol);
    
    hsl.r = mod(hsl.r + hue, 1.0);
    hsl.g = clamp(hsl.g + sat, 0.0, 1.0);
    hsl.b = clamp(hsl.b + luminance, 0.0, 1.0);
    
    vec4 col = hslToRgb(hsl);
	col.r = col.r*tintR;
	col.g = col.g*tintG;
	col.b = col.b*tintB;
    gl_FragColor = col;
}
