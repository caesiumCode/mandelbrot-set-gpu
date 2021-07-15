uniform int mode;   // 0: Default
                    // 1: Debug

uniform vec2 resolution; // Resoltion of the texture
uniform vec3 zoom; // Zoom parameters (coordinates of the center, scale factor)
uniform int limit; // Maximum number of iteration of the sequence

uniform sampler2D previous_state; // Texture previously computed
uniform vec4 previous_state_position; // Position of the previous texture relative to the current one
uniform bool previous_state_flag; // true if we use the previous state, false otherwise


// Return the number of iteration of the sequence before it escapes the "magnitude 2 border"
int escapeTime(vec2 c) {
    // Simple geometry optimization
    // Check if c is in the main cardioid/bulb
    float d_cx = c.x - 0.25;
    float cy2 = c.y*c.y;
    float q = d_cx*d_cx + cy2;
    if (q*(q + d_cx) <= 0.25*cy2 || (c.x + 1.0)*(c.x + 1.0) + cy2 < 0.0625)
        return limit;
    
    // Escape time algorithm
    int n = 0;
    float x = 0.0, x2 = 0.0;
    float y = 0.0, y2 = 0.0;
    while (x2 + y2 <= 4.0 && n < limit) {
        // Series of arithmetic manipulation that need 3 multiplications
        // instead of 5 for the naive method
        y = (x + x) * y + c.y;
        x = x2 - y2 + c.x;
        x2 = x * x;
        y2 = y * y;
        
        n++;
    }
    
    return n;
}

// Return an estimation of the distance between the input and the mandelbrot set
float distanceEstimation(vec2 c) {
    int n = 0;
    vec2 z = vec2(0.0);
    vec2 dz = vec2(1.0, 0.0);
    
    while (dot(z,z) <= 1.0e+10 && n < limit) {
        dz = vec2(2.0*(z.x*dz.x - z.y*dz.y) + 1.0, 2.0*(z.x*dz.y + z.y*dz.x));
        z = vec2(z.x*z.x - z.y*z.y + c.x, 2.0*z.x*z.y + c.y);
        
        n++;
    }
    
    if (dot(z,z) <= 4.0)
        return 0.0;
    else {
        float norm2 = dot(z,z), dnorm2 = dot(dz,dz);
        float norm = sqrt(norm2), dnorm = sqrt(dnorm2);
        return norm*log(norm)/dnorm;
    }
}

// Convert the number of iteration to a color
vec3 speedToColor(int n) {
    if (n >= limit)
        return vec3(0.0);
    else
        return 0.5 + 0.5*cos(10.0*sqrt(float(n)/float(limit)) + vec3(3.0, 3.5, 4.0));
}



// Convert a distance to the Mandelbrot set into a color
vec3 distanceToColor(float d) {
    if (d == 0.0)
        return vec3(0.0);
    else
        return 0.5 + 0.5*cos(10.0*pow(d/zoom.z, 0.2) + vec3(-0.5, 0.0, 0.5));
}

// Return true if the current pixel can also be found in the previous state
bool isInPreviousTexture(vec2 coord) {
    return coord.x >= previous_state_position.x &&
            coord.x < previous_state_position.x + previous_state_position.z &&
            coord.y >= previous_state_position.y &&
            coord.y < previous_state_position.y + previous_state_position.w;
}

// Main function of the shader
void main( void ) {
    vec3 color = vec3(0.0);
    
    // Previous state optimization
    if (previous_state_flag && isInPreviousTexture(gl_FragCoord.xy)) {
        
        vec2 texture_coord = vec2(gl_FragCoord.x-previous_state_position.x,
                                  resolution.y - (gl_FragCoord.y-previous_state_position.y));
        
        color = texture2D(previous_state, texture_coord/resolution).xyz;
        
        if (mode == 1)
            color.x = 0.0;
    }
    // Calculate the color from scratch
    else {
        // Map the coordinates of pixels into a normalized space such that -0.5 < x < 0.5
        vec2 normalized_coord = gl_FragCoord.xy/resolution.x - vec2(0.5, 0.5*resolution.y/resolution.x);
        vec2 complex_coord = zoom.xy + zoom.z * normalized_coord;
        
        if (mode == 0) {
            int iter = escapeTime(complex_coord);
            
            color = speedToColor(iter);
        } else if (mode == 1) {
            float dist = distanceEstimation(complex_coord);
            
            color = distanceToColor(dist);
        }
    }
    
    // Output
    gl_FragColor = vec4(color, 1.0);
}
