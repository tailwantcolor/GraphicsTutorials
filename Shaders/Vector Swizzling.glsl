fragColour = texture(diffuseTex, IN.texCoord).rgba;
fragColour = texture(diffuseTex, IN.texCoord).xyzw;
fragColour = texture(diffuseTex, IN.texCoord).rgzw;
fragColour = texture(diffuseTex, IN.texCoord).bgra;
fragColour = texture(diffuseTex, IN.texCoord).xxxw;