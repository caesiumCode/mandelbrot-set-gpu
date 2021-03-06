uniform int nb_directions; // Number of directions
uniform vec2 directions[8]; // Directions for seacrhing a color gradient

uniform vec2 resolution; // Resolution of the texture
uniform vec3 zoom; // Zoom parameters (coordinates of the center, scale factor)
uniform int limit; // Maximum number of iteration of the sequence

uniform sampler2D previous_state; // Texture previously computed
uniform vec4 previous_state_position; // Position of the previous texture relative to the current one
uniform bool previous_state_flag; // true if we use the previous state, false otherwise


// Return the number of iteration of the sequence before it escapes the "magnitude 2 border"
vec3 escapeTime(vec2 c) {
    // Simple geometry optimization
    // Check if c is in the main cardioid/bulb
    float d_cx = c.x - 0.25;
    float cy2 = c.y*c.y;
    float q = d_cx*d_cx + cy2;
    if (q*(q + d_cx) <= 0.25*cy2 || (c.x + 1.0)*(c.x + 1.0) + cy2 < 0.0625)
        return vec3(float(limit), 0.0, 0.0);
    
    // Escape time algorithm
    int n = 0;
    float x = 0.0, x2 = 0.0, x2prev = 0.0;
    float y = 0.0, y2 = 0.0, y2prev = 0.0;
    while (x2 + y2 <= 4.0 && n < limit) {
        // Series of arithmetic manipulation that need 3 multiplications
        // instead of 5 for the naive method
        y = (x + x) * y + c.y;
        x = x2 - y2 + c.x;
        x2prev = x2;
        y2prev = y2;
        x2 = x * x;
        y2 = y * y;
        
        n++;
    }
    
    return vec3(float(n), sqrt(x2prev + y2prev), sqrt(x2 + y2));
}

// Convert the number of iteration to a color
vec3 speedToColor(vec3 iter) {
    float n = iter.x;
    float flimit = float(limit);
    if (n >= flimit)
        return vec3(0.0);
    else {
        n += (2.0 - iter.y)/(iter.z - iter.y) - 1.0;
        return 0.5 + 0.5*cos(10.0*sqrt(n/flimit) + vec3(3.0, 3.5, 4.0));
    }
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
    
    vec3 iter = escapeTime(complex_coord);
    vec3 color = speedToColor(iter);
    
    return color;
}

// Main function of the shader
void main( void ) {
    vec3 color = vec3(0.0);
    
    // Previous state optimization
    if (previous_state_flag && isInPreviousTexture(gl_FragCoord.xy)) {
        
        float factor = resolution.x / previous_state_position.z;
        vec2 texture_coord = vec2(factor * (gl_FragCoord.x - previous_state_position.x),
                                  resolution.y - factor * (gl_FragCoord.y - previous_state_position.y));
        
        if (factor < 1.0 && checkColorGradient(texture_coord))
            color = computeColor(gl_FragCoord.xy);
        else
            color = texture2D(previous_state, texture_coord/resolution).xyz;
        
    } else color = computeColor(gl_FragCoord.xy);
    
    // Output
    gl_FragColor = vec4(color, 1.0);
}
