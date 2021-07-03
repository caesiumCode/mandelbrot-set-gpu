uniform vec2 resolution;

float isInMandelbrotSet(vec2 c) {
    const float LIMIT = 100.;
    
    float n = 0.;
    vec2 z = vec2(0., 0.);
    
    while (z.x*z.x + z.y*z.y <= 4. && n < LIMIT) {
        z = vec2(z.x*z.x - z.y*z.y + c.x, 2.*z.x*z.y + c.y);
        n++;
    }
    
    return n/LIMIT;
}

void main( void ) {
    vec2 normalized_coord = gl_FragCoord.xy/resolution.x - vec2(.5, .5*resolution.y/resolution.x);
    
    float speed = isInMandelbrotSet(5.*normalized_coord);
    
    gl_FragColor = vec4(speed, speed, speed, 1);
}
