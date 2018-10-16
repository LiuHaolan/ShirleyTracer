#include "parsing.h"




bool viParseFile(FILE* f, World* ptr) {
	int ch;

	while ((ch = getc(f)) != EOF)
	{
		switch (ch)
		{
		case ' ':   /* white space */
		case '\t':
		case '\n':
		case '\f':
		case '\r':
			continue;
		case '#':   /* comment */
		case '%':   /* comment */
			parseComment(f); /* ok */
			break;
		case 'v':   /* view point */
			parseViewpoint(f, ptr); /* ok */
			break;
		case 'l':   /* light source */
			parseLight(f, ptr); /* ok */
			break;
		case 'b':   /* background color */
			parseBackground(f, ptr); /* ok */
			break;
		case 'f':   /* fill material */
	//		parseFill(f); /* ok */
			break;
		case 'c':   /* cylinder or cone */
	//		parseCone(f); /* ok */
			break;
		case 's':   /* sphere */
	//		parseSphere(f); /* ok */
			break;
		case 'p':   /* polygon or patch */
	//		parsePoly(f);
			break;
		case 'i':   /* include another file */
	//		parseInclude(f);  /* ok */
			break;
		case 'd':   /* detail level of file (used to exclude objects from rendering) */
	//		parseDetailLevel(f); /* ok */
			break;
		case 't':  /* textured triangle, or texture tripatch, or animated triangle */
	//		parseTextureStuff(f);
			break;
		case 'x':  /* transform */
	//		parseXform(f);
			break;
		case '}':
			//		viEndXform();
			break;
		case 'a':  /* animation parameters */
	//		parseA(f);
			break;
		case 'k':  /* key frames for transform (or the camera) */
	//		parseKeyFrames(f);
			break;
		case 'm':  /* triangle mesh */
	//		parseMesh(f);
			break;
		default:    /* unknown */
			printf("unknown NFF primitive code: %c\n", ch);
			//		exit(1);
		}
	}
	return true;
}