uniform int mode;   // 0: Default
                    // 1: Debug

uniform int nb_directions; // Number of directions
uniform vec2 directions[8]; // Directions for seacrhing a color gradient

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

// Convert the number of iteration to a color
vec3 speedToColor(int n) {
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

// Return true if there is a color gradient
bool checkColorGradient(vec2 coord) {
    vec4 reference_color = texture2D(previous_state, coord/resolution);
    
    for (int i = 0; i < nb_directions; i++) {
        if ((coord + directions[i]).x >= 0.0 && (coord + directions[i]).x < resolution.x &&
            (coord + directions[i]).y >= 0.0 && (coord + directions[i]).y < resolution.y)
            if (texture2D(previous_state, (coord + directions[i])/resolution) != reference_color)
                return true;
    }
    
    return false;
}

// Return the color of a pixel by iterating a complex sequence
vec3 computeColor(vec2 coord) {
    // Map the coordinates of pixels into a normalized space such that -0.5 < x < 0.5
    vec2 normalized_coord = coord/resolution.x - vec2(0.5, 0.5*resolution.y/resolution.x);
    vec2 complex_coord = zoom.xy + zoom.z * normalized_coord;
    
    int iter = escapeTime(complex_coord);
    return speedToColor(iter);
}

// Main function of the shader
void main( void ) {
    vec3 color = vec3(0.0);
    
    // Previous state optimization
    if (previous_state_flag && isInPreviousTexture(gl_FragCoord.xy)) {
        
        float factor = resolution.x / previous_state_position.z;
        vec2 texture_coord = vec2(factor * (gl_FragCoord.x - previous_state_position.x),
                                  resolution.y - factor * (gl_FragCoord.y - previous_state_position.y));
        
        if (factor < 1.0 && checkColorGradient(texture_coord)) {
            color = computeColor(gl_FragCoord.xy);
            if (mode == 1)
                color.x = 1.0; // add red
        } else {
            color = texture2D(previous_state, texture_coord/resolution).xyz;
            if (mode == 1)
                color.y = 1.0; // add green
        }
    } else {
        color = computeColor(gl_FragCoord.xy);
        if (mode == 1)
            color.z = 1.0; // add blue
    }
    
    // Output
    gl_FragColor = vec4(color, 1.0);
}
