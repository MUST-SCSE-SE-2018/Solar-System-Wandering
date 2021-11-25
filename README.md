# Solar-System-Wandering

> After learning the course CS104, Kennard totally spent 6 days to finish the final project ( 2 days for planet drawing and texture, 
> 2 days for light setting and skybox texture, 1 day for player control setting and 1 day for report ). 
> This project looks like an tiny game and the player can wander around the solar system by controlling a spacecraft.

------

### Demo
+ [Video Demo](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/video.mp4)
+ Start

![demo1](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/demo1.png)

+ Move Forward

![demo2](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/demo2.png)

+ Move Backward

![demo3](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/demo3.png)

+ Scale Up

![demo4](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/demo4.png)

+ Move upward & Turn Right

![demo5](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/demo5.png)

------

### Development Environment

| Description | Specification |
|:---:|:---:|
| System | Windows 10 |
| Language | C++, GLSL |
| IDE | Visual Studio 2019 ( Community ) v16.7.5 |

------

### Deployment
1. Create a project and copy all files into the root directory.

![tutorial1](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial1.png)

2. Click **Show All Files**.

![tutorial2](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial2.png)

3. Select all files except those in **deployment** , right click and choose **Include In Project**.

![tutorial3](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial3.png)

4. Find **spacecraft.obj**, right click and choose **Properties**.

![tutorial4](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial4.png)

5. Set the value of **Excluded from Build** to **Yes**.

![tutorial5](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial5.png)

6. Find project -> Properties -> C/C++ -> General, set the include directories as **.../deployment/include**.

![tutorial6](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial6.png)

7. Find project -> Properties -> Linker -> General, set the additional library directories as **.../deployment/lib**.

![tutorial7](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial7.png)

8. Find project -> Properties -> Linker -> Input, set the additional dependencies as **glfw3.lib glew32.lib soil2-debug.lib opengl32.lib**.

![tutorial8](https://kennardwang.github.io/ImageSource/Solar-System-Wandering/tutorial8.png)

9. Now you can start to compile **main.cpp** and run.

------

### File Description
|File Name|Description|
|:---:|---|
|main.cpp|The main program to handle events of solar system|
|Sphere.h|Declaration of functions in Sphere.cpp|
|Sphere.cpp|Utilities of sphere creation|
|Torus.h|Declaration of functions in Torus.cpp|
|Torus.cpp|Utilities of torus creation|
|Utils.h|Declaration of functions in Utils.cpp|
|Utils.cpp|Some utilities|
|ImportedModel.h|Declaration of functions in ImportedModel.cpp|
|ImportedModel.cpp|Utilities of model import|
|vertShader.glsl|Vertext shader of among planet, spacecraft and lighting|
|fragShader.glsl|Fragment shader of among planet, spacecraft and lighting|
|vertShader_skybox.glsl|Vertext shader of skybox|
|fragShader_skybox.glsl|Fragment shader of skybox|
|texture|Texture images|
|spacecraft.obj|Model object|

------

### Design Outline
+ Planet
  + Description
  > I draw the sun and other 8 planets ( Including Mercury, Venus, Earth with Moon, Mars, Jupiter, Saturn, Uranus and Neptune. By the way, the Pluto is too small to see in program so that I omit it ). Each of the planets has some special features.
  
  + Feature
    1. Revolution
    > Each planet has different revolution speed. As we all known, the Mercury has the highest revolution speed because it is the nearest planet to the Sun. And the Neptune is the lowest one. You could see clearly in my video demo.
      
    2. Rotation
    > Each planet has different rotation direction and rotation speed, as well rotation axis. For example, the Venus rotates from the east to the west and others are different. Additionally, the rotation axis of the Uranus is x axis because the Uranus rotates while 'lying'. The rotation axis of Mars, Neptune, Earth and Saturn has a constant angle to the y axis.
       
    3. Size
    > Each planet has different size. Except the Sun, the Jupiter is the largest and the Mercury is the smallest.
       
    4. Other special features
    > The implementation of the Saturn ring, which is made of Torus.           
  
  + Code Fragment ( *Use the Saturn as an example* )
    + Revolution & Rotation & Size
       
    ```
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.4) * 39, 0.0f, cos((float)currentTime * 0.4) * 39));  // planet revolution
    tmpMat = mvStack.top();
    mvStack.push(mvStack.top());

    // rotation axis
    mvStack.top() *= rotate(glm::mat4(1.0f), toRadians(26.7), glm::vec3(0.0, 0.0, 1.0));  
    // self rotation
    mvStack.top() *= rotate(glm::mat4(1.0f), (float)currentTime * 3, glm::vec3(0.0, 1.0, 0.0)); 
    // size of the saturn 
    mvStack.top() *= scale(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 4.0f));      
    ```
         
    + Bind VBO & Apply Texture
       
    ```
    //-----------------------  sphere == saturn
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, saturnTexture);  // texture
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());

    mvStack.pop();
    mvStack.pop();
    ```
         
    + The Saturn Ring
       
    ```
    // saturn ring
    vMat = tmpMat;
    vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0f, 12.0f));
    // rotation axis
    vMat *= rotate(glm::mat4(1.0f), toRadians(26.7), glm::vec3(0.0, 0.0, 1.0));  
    vMat *= scale(glm::mat4(1.0f), glm::vec3(12.0f, 1.0f, 12.0f));
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX, torLocY, torLocZ));

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));      
    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[8]);
    glDrawElements(GL_TRIANGLES, numTorusIndices, GL_UNSIGNED_INT, 0);
    ```
  
+ Light
  + Description
  > As we all known, the sun generates the sunlight to light up all the planets. Because of the far distance, the sunlight can be considered as a direct light, which is a parallel emission without highlight point. That's why we use direct light ADS model here and omit the highlight point.
  
  + Feature
  > Each planet has both bright side and dark side according to different light angles of the sun, which enhances the reality of view.
  
  + Code Fragment ( *Add following code into the former code fragment if you want to apply light to an object* )
    + ***main.cpp***
    
    ```
    vMat = mvStack.top();
    vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.4) * 39, 0.0f, cos((float)currentTime * 0.4) * 39)) * rotate(glm::mat4(1.0f), toRadians(26.7), glm::vec3(0.0, 0.0, 1.0)) * rotate(glm::mat4(1.0f), (float)currentTime * 3, glm::vec3(0.0, 1.0, 0.0)); // apply data of the Saturn
    installLights(vMat);
    mvMat = vMat * mMat;
    invTrMat = glm::transpose(-glm::inverse(mvMat));
    ```
    
    + ***fragShader.glsl***
    
    ```
    void main(void)
    {    	
      // normalize the light, normal, and view vectors:
      vec3 L = normalize(varyingLightDir);
      vec3 N = normalize(varyingNormal);
      vec3 V = normalize(-varyingVertPos);

      // compute light reflection vector, with respect N:
      vec3 R = normalize(reflect(-L, N));

      // get the angle between the light and surface normal:
      float cosTheta = dot(L,N);

      // angle between the view vector and reflected light:
      float cosPhi = dot(V,R);

      // compute ADS contributions (per pixel):
      // apply both texture and ADS lighting model
      vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz * vec3(texture(s,tc));  
      vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0) * vec3(texture(s,tc)); // use matrix product
      vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess);

      fragColor = vec4((ambient + diffuse + specular), 1.0);

    }
    ```
      
    + ***vertShader.glsl***
    
    ```
    void main(void)
    {	
      tc = tex_coord;
      varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz;
      varyingLightDir = light.position - varyingVertPos;
      varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;

      gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
    }
    ```

+ Galaxy Background
  + Description
  > I use the skybox texture-mapping method provided by textbook and add automatic rotation that you can see the surround of skybox.
  
  + Feature
  > Background will rotate automatically.
  
  + Code Fragment
    + Main code used in ***void display(GLFWwindow\* window, double currentTime)***
    
    ```
    void set_skybox(double currentTime) {

      glUseProgram(renderingProgramCubeMap);

      vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

      // skybox rotation
      vMat *= rotate(glm::mat4(1.0f), (float)currentTime * 0.1f, glm::vec3(0.0, 1.0, 0.0)); 
      mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));
      mvMat = vMat * mMat;

      mvLoc = glGetUniformLocation(renderingProgramCubeMap, "mv_matrix");
      projLoc = glGetUniformLocation(renderingProgramCubeMap, "proj_matrix");

      glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

      glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(1);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, skyboxTexture);

      glEnable(GL_CULL_FACE);
      glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
      glDisable(GL_DEPTH_TEST);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glEnable(GL_DEPTH_TEST);
    }
    ```
    
    + Please do not forget to initialize and load first in ***init()***
    
    `renderingProgramCubeMap = Utils::createShaderProgram("vertShader_skybox.glsl", "fragShader_skybox.glsl");`
      
    ```
    // load skybox
	  skyboxTexture = Utils::loadTexture("texture/galaxy.jpg");
	  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    ```
      
    + ***fragShader_skybox.glsl***
    
    ```
    #version 430

    in vec2 tc;
    out vec4 fragColor;

    uniform mat4 mv_matrix;
    uniform mat4 proj_matrix;
    layout (binding = 0) uniform sampler2D s;

    void main(void)
    {
      fragColor = texture(s,tc);
    }
    ```
      
    + ***vertShader_skybox.glsl***
    
    ```
    #version 430

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 tex_coord;
    out vec2 tc;

    uniform mat4 mv_matrix;
    uniform mat4 proj_matrix;
    layout (binding = 0) uniform sampler2D s;

    void main(void)
    {
      tc = tex_coord;
      gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
    } 
    ```
   
+ Player Control
  + Description
  > I import a spacecraft model as a player view, combine with the keyboard input and mouse event, provide players with a feeling of travelling in the solar system.
  
  + Control Event
     
    |Command|Event|
    |:---:|---|
    |KEY_Q|Quit|
    |KEY_A|Turn Left|           
    |KEY_D|Turn Right| 
    |KEY_W|Turn Up|
    |KEY_S|Turn Down|
    |KEY_Z|Move Forward|
    |KEY_C|Move Backward|
    |KEY_UP|Move Upward|
    |KEY_DOWN|Move Downward|
    |KEY_LEFT|Move Left|
    |KEY_RIGHT|Move Right|
    |MOUSE_SCROLLUP|Scale Up|
    |MOUSE_SCROLLDOWN|Scale Down|
    |WINDOW_RESIZE|Automatic Adaptation|
    |CURSOR_Hide ( optional )|Hide Cursor|
  
  + Code Fragment
    + Import Model
    
    ```
    ImportedModel myModel("spacecraft.obj");  // global
      
    // set model object vertices
	  std::vector<glm::vec3> vert3 = myModel.getVertices();
	  std::vector<glm::vec2> tex3 = myModel.getTextureCoords();
	  std::vector<glm::vec3> norm3 = myModel.getNormals();

	  std::vector<float> pvalues3;
	  std::vector<float> tvalues3;
	  std::vector<float> nvalues3;

	  for (int i = 0; i < myModel.getNumVertices(); i++) {
	    pvalues3.push_back((vert3[i]).x);
		  pvalues3.push_back((vert3[i]).y);
		  pvalues3.push_back((vert3[i]).z);
		  tvalues3.push_back((tex3[i]).s);
		  tvalues3.push_back((tex3[i]).t);
		  nvalues3.push_back((norm3[i]).x);
		  nvalues3.push_back((norm3[i]).y);
		  nvalues3.push_back((norm3[i]).z);
	  }
    ```
              
    + Keyboard Event
    
    ```
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

      if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);  // Q to quit
      if (action == GLFW_PRESS)  // while press
        keys[key] = true;
      else if (action == GLFW_RELEASE)  // while release
        keys[key] = false;
    }
    
    void spacecraftMove(){

      GLfloat moveSpeed = 0.15f;

      if (keys[GLFW_KEY_LEFT])
        camMovX += moveSpeed;  // move left
      if (keys[GLFW_KEY_RIGHT])
        camMovX -= moveSpeed;  // move right
      if (keys[GLFW_KEY_UP])
        camMovY -= moveSpeed;  // move up
      if (keys[GLFW_KEY_DOWN])
        camMovY += moveSpeed;  // move down
      if (keys[GLFW_KEY_Z])
        camMovZ += moveSpeed;  // move forward
      if (keys[GLFW_KEY_C])
        camMovZ -= moveSpeed;  // move back
      if (keys[GLFW_KEY_W])
        rotUDOffset -= 0.005f;  // rotate up
      if (keys[GLFW_KEY_S])
        rotUDOffset += 0.005f;  // rotate down
      if (keys[GLFW_KEY_A])
        rotLROffset -= 0.005f;  // rotate left
      if (keys[GLFW_KEY_D])
        rotLROffset += 0.005f;  // rotate right
    }
    ```
      
    + Mouse Event
    
    ```
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

      if (scaleOffset >= 0.5f && scaleOffset <= 1.2f)  // scroll scale
        scaleOffset -= yoffset * 0.02f;
      if (scaleOffset <= 0.5f)
        scaleOffset = 0.5f;
      if (scaleOffset >= 1.2f)
        scaleOffset = 1.2f;

      pMat = glm::perspective(scaleOffset, aspect, 0.1f, 1000.0f);  // modify project matrix
    }
    ```
      
    + Apply ALL Callback Function
    
    ```
    // call back event
	  glfwSetWindowSizeCallback(window, window_size_callback);
	  glfwSetKeyCallback(window, key_callback);
	  glfwSetScrollCallback(window, scroll_callback);
	  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // optional: hide the cursor
      
    while (!glfwWindowShouldClose(window)) {	
	    spacecraftMove();
		  display(window, glfwGetTime());	
		  glfwSwapBuffers(window);
		  glfwPollEvents();
	  }
    ```

------

### License  
  + [MIT License](https://github.com/KennardWang/Solar-System-Wandering/blob/master/LICENSE)

------

### Author
  + Kennard Wang ( 2020.12.5 )
