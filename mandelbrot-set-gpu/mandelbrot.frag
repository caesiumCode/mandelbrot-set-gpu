uniform vec2 resolution; // Resoltion of the texture
uniform vec3 zoom; // Zoom parameters (coordinates of the center, scale factor)
uniform int limit; // Maximum number of iteration of the sequence

uniform sampler2D previous_state; // Texture previously computed
uniform vec4 previous_state_position; // Position of the previous texture relative to the current one
uniform bool previous_state_flag; // true if we use the previous state, false otherwise


// Return the number of iteration of the sequence before it escapes the "magnitude 2 border"
int isInMandelbrotSet(vec2 c) {
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

// Convert the number of iteration to a color
vec3 toColor(int n) {
    if (n >= limit)
        return vec3(0.0);
    else
        return 0.5 + 0.5*cos(10.0*sqrt(float(n)/float(limit)) + vec3(3.0, 3.5, 4.0));
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
    }
    // Calculate the color from scratch
    else {
        // Map the coordinates of pixels into a normalized space such that -0.5 < x < 0.5
        vec2 normalized_coord = gl_FragCoord.xy/resolution.x - vec2(0.5, 0.5*resolution.y/resolution.x);
        
        int iter = isInMandelbrotSet(zoom.xy + zoom.z * normalized_coord);
        color = toColor(iter);
    }
    
    // Output
    gl_FragColor = vec4(color, 1.0);
}
