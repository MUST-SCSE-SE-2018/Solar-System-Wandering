# Solar-System-Wandering

<div align="center">

  [![description](https://img.shields.io/badge/project-Individual-FFB7C5?style=for-the-badge)](https://github.com/KennardWang/Solar-System-Wandering)
  &nbsp;
  [![stars](https://img.shields.io/github/stars/KennardWang/Solar-System-Wandering?style=for-the-badge&color=FDEE21)](https://github.com/KennardWang/Solar-System-Wandering/stargazers)
  &nbsp;
  [![forks](https://img.shields.io/github/forks/KennardWang/Solar-System-Wandering?style=for-the-badge&color=white)](https://github.com/KennardWang/Solar-System-Wandering/forks)
  &nbsp;
  [![contributors](https://img.shields.io/github/contributors/KennardWang/Solar-System-Wandering?style=for-the-badge&color=8BC0D0)](https://github.com/KennardWang/Solar-System-Wandering/graphs/contributors)
  
  <img src="https://img.shields.io/badge/windows-0078D6?logo=windows&logoColor=white&style=for-the-badge" />
  &nbsp;
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  &nbsp;
  <img src="https://img.shields.io/badge/OpenGL-696969?style=for-the-badge&logo=opengl" />
  &nbsp;
  <img src="https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white" />
</div>

<br>

<div align=center>
  <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/demo1.png" width="70%" />
</div>

<br>
<div align=center>
  <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/demo2.png" width="35%" />
  &nbsp;
  <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/demo3.png" width="35%" />
</div>

<div align=center>
  <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/demo4.png" width="35%" />
  &nbsp;
  <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/demo5.png" width="35%" />
</div>

<br>

It is a project of MUST course ***CS104-Computer Graphics***, which is a game developed in C++ and OpenGL. The game allows users to control a spacecraft and wander around the solar system.

+ [Watch the full demo](https://kennardwang.github.io/Image-Source/Solar-System-Wandering/video.mp4)



## Table of Contents

- [Development Environment](#development-environment)
- [Install](#install)
- [Usage](#usage)
- [File Description](#file-description)
- [Design](#design)
- [Releases](#releases)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)



## Development Environment

| <!--> | <!--> |
|:---:|:---:|
| System | Windows 10 x64 |
| Language | C++, GLSL |
| IDE | Visual Studio 2019 Community v16.7.5 |



## Install

1. Download the repository by `git clone https://github.com/MUST-SCSE-SE-2018/Solar-System-Wandering.git`.
2. [Download](https://github.com/KennardWang/Solar-System-Wandering/releases) the **deployment** folder and unzip.
3. Create a project and copy all files into the root directory.

  <div align=center>
     <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial1.png" width="60%" />
  </div>

4. Click **Show All Files**, and select all files except those in **deployment**. Right-click and choose **Include In Project**. Find `spacecraft.obj`, right click and choose **Properties**.

  <div align=center>
     <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial2.png" width="25%" />
     &nbsp;
     <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial3.png" width="25%" />
     &nbsp;
     <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial4.png" width="25%" />
  </div>


5. Set the value of **Excluded from Build** to **Yes**. Find ***project -> Properties -> C/C++ -> General***, and set the include directories as `.../deployment/include`. Find ***project -> Properties -> Linker -> General***, and set the additional library directories as `.../deployment/lib`. Find ***project -> Properties -> Linker -> Input***, and set the additional dependencies as `glfw3.lib glew32.lib soil2-debug.lib opengl32.lib`.

  <div align=center>
    <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial5.png" width="35%" />
    &nbsp;
    <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial6.png" width="35%" />
  </div>

  <div align=center>
    <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial7.png" width="35%" />
    &nbsp;
    <img src="https://kennardwang.github.io/Image-Source/Solar-System-Wandering/tutorial8.png" width="35%" />
  </div>



## Usage

Compile all the files and run the executable file. The keyboard and mouse events are listed below.
       
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



## File Description

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



## Design
+ Planet
  + Description
  > I draw the sun and the other 8 planets (including Mercury, Venus, Earth with Moon, Mars, Jupiter, Saturn, Uranus and Neptune. By the way, Pluto is too small to see in the program so I omit it). Each of the planets has some special features.
  
  + Features
    1. Revolution
    > Each planet has a different revolution speed. As we all know, Mercury has the highest revolution speed because it is the nearest planet to the Sun. And the Neptune is the lowest one. You could see this clearly in my video demo.
      
    2. Rotation
    > Each planet has a different rotation direction and speed, as well as the rotation axis. For example, Venus rotates from the east to the west and others are different. Additionally, the rotation axis of the Uranus is the x-axis because the Uranus rotates while 'lying'. The rotation axis of Mars, Neptune, Earth and Saturn has a constant angle to the y-axis.
       
    3. Size
    > Each planet has a different size. Except for the Sun, Jupiter is the largest and the Mercury is the smallest.
       
    4. Other special features
    > The implementation of the ring of Saturn, which is made of Torus.           
  
  + Code fragment (use Saturn as an example)
    + Revolution & rotation & size
       
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
         
    + Bind VBO & apply texture
       
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
         
    + The ring of Saturn
       
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
  > As we all know, the sun generates sunlight to light up all the planets. Because of the far distance, the sunlight can be considered direct light, which is a parallel emission without a highlight point. That's why we use the direct light ADS model here and omit the highlight point.
  
  + Features
  > Each planet has both a bright side and a dark side according to different light angles of the sun, which enhances the reality of view.
  
  + Code fragment (add the following code into the former code fragment if you want to apply light to an object)
    + In `main.cpp`
    
	    ```
	    vMat = mvStack.top();
	    vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime * 0.4) * 39, 0.0f, cos((float)currentTime * 0.4) * 39)) * rotate(glm::mat4(1.0f), toRadians(26.7), glm::vec3(0.0, 0.0, 1.0)) * rotate(glm::mat4(1.0f), (float)currentTime * 3, glm::vec3(0.0, 1.0, 0.0)); // apply data of the Saturn
	    installLights(vMat);
	    mvMat = vMat * mMat;
	    invTrMat = glm::transpose(-glm::inverse(mvMat));
	    ```
    
    + In `fragShader.glsl`
    
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
      
    + In `vertShader.glsl`
    
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
  > I use the skybox texture-mapping method provided by the textbook and add automatic rotation so that you can see the surround of the skybox.
  
  + Features
  > Background will rotate automatically.
  
  + Code fragment
    + Main code used in `void display(GLFWwindow\* window, double currentTime)`
    
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
    
    + Do not forget to initialize and load first in `init()`
          
	    ```
	    // initialization
	    renderingProgramCubeMap = Utils::createShaderProgram("vertShader_skybox.glsl", "fragShader_skybox.glsl");
	    
	    // load skybox
	    skyboxTexture = Utils::loadTexture("texture/galaxy.jpg");
	    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	    ```
      
    + In `fragShader_skybox.glsl`
    
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
      
    + In `vertShader_skybox.glsl`
    
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
   
+ Player control  
  + Code fragment
    + Import model
    
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
              
    + Keyboard events
    
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
      
    + Mouse events
    
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
      
    + Apply all the callback function
    
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



## Releases

[![badge](https://img.shields.io/badge/release-Latest-FF7800)](https://github.com/KennardWang/Solar-System-Wandering/releases/tag/environment)



## Maintainers

![badge](https://img.shields.io/badge/maintenance-NO-EF2D5E) [@KennardWang](https://github.com/KennardWang)



## Contributing

Feel free to [open an issue](https://github.com/KennardWang/Solar-System-Wandering/issues) or submit [PRs](https://github.com/KennardWang/Solar-System-Wandering/pulls).



## License

[![license](https://img.shields.io/github/license/KennardWang/Solar-System-Wandering)](LICENSE) © Kennard Wang ( 2020.12.5 )
