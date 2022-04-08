#version 430

in float myIsPoint;
in float myIsPolygon;

out vec4 color;

void main() {

    
    if(myIsPoint>0.5){
         if(length(gl_PointCoord-vec2(0.5)) > 0.5)
            discard;
        color = vec4(0.0,1.0,0.0,1.0);
    }else if(myIsPolygon>0.5){
                color = vec4(0.0,0.0,1.0,1.0);
    }
    else{
        color = vec4(1.0,0.0,0.0,1.0);
    }
   

} 