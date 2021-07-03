uniform vec2 resolution;

bool isInMandelbrotSet(vec2 c) {
    const int LIMIT = 100;
    
    int n = 0;
    vec2 z = vec2(0., 0.);
    
    while (z.x*z.x + z.y*z.y <= 4. && n < LIMIT) {
        z = vec2(z.x*z.x - z.y*z.y + c.x, 2.*z.x*z.y + c.y);
        n++;
    }
    
    if (n == LIMIT)
        return true;
    else
        return false;
}

void main( void ) {
    vec2 normalized_coord = gl_FragCoord.xy/resolution - vec2(.5, .5);
    
    if (isInMandelbrotSet(3.*normalized_coord))
        gl_FragColor = vec4(1, 1, 1, 1);
    else
        gl_FragColor = vec4(0, 0, 0, 1);
}
