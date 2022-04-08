#version 430

in float myIsPoint;

out vec4 color;

void main() {

    
    if(myIsPoint>0.5){
         if(length(gl_PointCoord-vec2(0.5)) > 0.5)
            discard;
        color = vec4(0.0,1.0,0.0,1.0);
    }
    else{
        color = vec4(1.0,0.0,0.0,1.0);
    }
   

} 