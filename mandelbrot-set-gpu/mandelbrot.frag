uniform vec2 resolution;
uniform vec3 zoom;
uniform float limit;

float isInMandelbrotSet(vec2 c) {
    float n = 0.;
    vec2 z = vec2(0., 0.);
    
    while (z.x*z.x + z.y*z.y <= 4. && n < limit) {
        z = vec2(z.x*z.x - z.y*z.y + c.x, 2.*z.x*z.y + c.y);
        n++;
    }
    
    return n/limit;
}

void main( void ) {
    vec2 normalized_coord = gl_FragCoord.xy/resolution.x - vec2(.5, .5*resolution.y/resolution.x);
    
    float speed = isInMandelbrotSet(zoom.xy + zoom.z * normalized_coord);
    
    gl_FragColor = vec4(speed, speed, speed, 1);
}
