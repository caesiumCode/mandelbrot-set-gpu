uniform vec2 resolution;
uniform vec3 zoom;
uniform int limit;


int isInMandelbrotSet(vec2 c) {
    float p = sqrt((c.x - 0.25)*(c.x - 0.25) + c.y*c.y);
    if (c.x < p - 2.0*p*p + 0.25 || (c.x + 1.0)*(c.x + 1.0) + c.y*c.y < 0.0625)
        return limit;
    
    int n = 0;
    vec2 z = vec2(0.0, 0.0);
    
    while (dot(z,z) <= 4.0 && n < limit) {
        z = vec2(z.x*z.x - z.y*z.y + c.x, 2.0*z.x*z.y + c.y);
        n++;
    }
    
    return n;
}

vec3 toColor(int n) {
    if (n >= limit)
        return vec3(0.0, 0.0, 0.0);
    else
        return 0.5 + 0.5*cos(10.0*sqrt(float(n)/float(limit)) + vec3(3.0, 3.5, 4.0));
}

void main( void ) {
    vec2 normalized_coord = gl_FragCoord.xy/resolution.x - vec2(0.5, 0.5*resolution.y/resolution.x);
    
    int iter = isInMandelbrotSet(zoom.xy + zoom.z * normalized_coord);
    vec3 col = toColor(iter);
    
    gl_FragColor = vec4(col, 1.0);
}
