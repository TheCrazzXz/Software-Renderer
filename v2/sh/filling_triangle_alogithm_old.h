float triangleLeftX(float y, float topVertexX, float xmin, float ymin, float ymax)
{
	float AD = ymax - ymin;
	float AF = y - ymin;
	float DC = topVertexX - xmin;

	//printf("AD = %f, AF = %f, DC = %f\n", AD, AF, DC);

	float FG = (DC * AF) / AD;

	return xmin + FG;
}
float triangleRightX(float y, float topVertexX, float xmax, float ymin, float ymax)
{
	float BE = ymax - ymin;
	float BF = y - ymin;
	float EC = xmax - topVertexX;

	//printf("AD = %f, AF = %f, DC = %f\n", AD, AF, DC);

	float FG = (EC * BF) / BE;

	return xmax - FG;
}

void fillTriangle(struct DrawVertexData triangle[], PixelBuffer* pixelBuffer)
{
	/*struct DrawVertexData triangle[3];
	memcpy(triangle, triangleToFill, 3 * sizeof(struct DrawVertexData));
	Matrix4 transform = getIdentityMatrix4();
	Vector3 axis = {0, 0, 1};
	transform = Matrix4_rotate(&transform, axis, M_PI);

	for(ulong i = 0 ; i < 3 ; i++)
	{
		Vector4 pos = {triangle[i].position.x, triangle[i].position.y, 0, 1};
		pos = Matrix4_multiplyVector(&transform, pos);
		triangle[i].position.x = pos.x;
		triangle[i].position.y = pos.y;
	} */

	float vertices_x[] = { triangle[0].position.x, triangle[1].position.x, triangle[2].position.x };
	float vertices_y[] = { triangle[0].position.y, triangle[1].position.y, triangle[2].position.y };
	
	

	// defining aabb
	float xmin = min(vertices_x, sizeof(vertices_x) / sizeof(float));
	float xmax = max(vertices_x, sizeof(vertices_x) / sizeof(float));

	float ymin = min(vertices_y, sizeof(vertices_y) / sizeof(float));

	float ymax = max(vertices_y, sizeof(vertices_y) / sizeof(float));
	printf("%f\n", ymax);
	//printf("ymax : %f\n", ymax);
	struct DrawVertexData* topVertex;
	bool foundTopVertex = false;
	for(ulong i = 0 ; i < 3 ; i++)
	{
		if(triangle[i].position.y == ymax)
		{
			//printf("position.y = %f\n", triangle[i].position.y);
			topVertex = &triangle[i];
			foundTopVertex = true;
		}
	}
	if(!foundTopVertex)
	{
		printf("<fillTriangle> ERROR : Couldn't determine top vertex coordinates\n");
		exit(-1);
	}

	contextVertex0 = triangle[0];
	contextVertex1 = triangle[1];

	//printf("pixels to draw : %f\n",  ((ymax - ymin) * (float)pixelBuffer->height));

	//contextVertex2 = triangle[1];
	// Loop through each horizontal scanline (each y coordinate horizontal pixel list in aabb)
	for(float y = ymin ; y < ymax ; y += Y_ADVANCE / (float) pixelBuffer->height)
	{
		Vector2 point_screen;
		// Loop though each pixel of the horizontal scanline
		//printf("x = %f\n", x);
		//for(float x = xmin ; x < xmax ; x += X_ADVANCE / (float) pixelBuffer->width)
		//{
			//Vector2 point = { x, y };
			//if(PointInTriangle(point, triangle[0].position, triangle[1].position, triangle[2].position))
			//{
				
				/*point_screen = planeCoordinatesToScreenCoordinates(point, pixelBuffer);
				//pixelAtPosition(point_screen.x, point_screen.y, color, pixelBuffer);

				(point_screen.x, point_screen.y, pixelBuffer);*/
				float leftPixelX = triangleLeftX(y, topVertex->position.x, xmin, ymin, ymax);
				float rightPixelX = triangleRightX(y, topVertex->position.x, xmax, ymin, ymax);
				//planPixel(leftPixelX, y, 0xffffffff, pixelBuffer);
				//planPixel(rightPixelX, y, 0xffffffff, pixelBuffer);

				//vertices2Color(vertex1->color, vertex2->color);
				float positions[] = { leftPixelX, y, rightPixelX, y };
				draw2DVertices(positions, 4, pixelBuffer);

				/*for(float x_cpy = point_screen.x ; x_cpy < point_screen.x + X_ADVANCE ; x_cpy++)
				{
					//pixelBuffer->buffer[(int)(point_screen.y*pixelBuffer->width + x_cpy)] = pixelBuffer->buffer[(int)(point_screen.y*pixelBuffer->width + point_screen.x)];
					for(float y_cpy = point_screen.y ; y_cpy < point_screen.y + Y_ADVANCE ; y_cpy++)
					{
						if(isInBoundsScreen(x_cpy, y_cpy, pixelBuffer))
						{
							pixelBuffer->buffer[(int)(y_cpy*pixelBuffer->width + x_cpy)] = pixelBuffer->buffer[(int)(point_screen.y*pixelBuffer->width + point_screen.x)];
						}
						
					}
				}*/

			//}
			
		//}
		

	}
}