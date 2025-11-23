/**
 * @file plain.h
 *
 * @brief Plain model
 *
 * @details  Command for rendering
 *           glDrawArrays(GL_TRIANGLES, 0, 6);
 *
 * @author Martin Nemec
 *
 * @year 2023
 **/

const float plain[] = {
	1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
	1.0f, 0.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,

	-1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f
};