/*
 The contents of this file are subject to the Mozilla Public License
 Version 1.1 (the "License"); you may not use this file except in
 compliance with the License. You may obtain a copy of the License at
 http://www.mozilla.org/MPL/

 Software distributed under the License is distributed on an "AS IS"
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 License for the specific language governing rights and limitations
 under the License.

 Alternatively, the contents of this file may be used under the terms
 of the GNU Lesser General Public license (the  "LGPL License"), in which case the
 provisions of LGPL License are applicable instead of those
 above.

 For feedback and questions about my Files and Projects please mail me,
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com
*/

/* This file is very bad style - and I know it! But this is the best
 * way to get the code fast and still understanable. The only
 * alternative I used before was massive copy and paste, which is
 * bollocks. */
 
/* ******************** */
/* draw_pixel functions */
/* ******************** */

#define draw_pixel_ztest_zset(x,y,z,color) \
	{if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
	{\
		spTargetPixel[(x) + (y) * spTargetScanLine] = (color);\
		spZBuffer[(x) + (y) * spTargetScanLine] = (z);\
	}}

#define draw_pixel_ztest(x,y,z,color) \
	{if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		spTargetPixel[(x) + (y) * spTargetScanLine] = (color);}

#define draw_pixel_zset(x,y,z,color) \
	{spTargetPixel[(x) + (y) * spTargetScanLine] = (color); \
	spZBuffer[(x) + (y) * spTargetScanLine] = (z);}

#define draw_pixel(x,y,color) \
	{spTargetPixel[(x) + (y) * spTargetScanLine] = (color);}
	
// + Pattern

#define draw_pixel_ztest_zset_pattern(x,y,z,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_ztest_zset(x,y,z,color)}

#define draw_pixel_ztest_pattern(x,y,z,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_ztest(x,y,z,color)}

#define draw_pixel_zset_pattern(x,y,z,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_zset(x,y,z,color)}

#define draw_pixel_pattern(x,y,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel(x,y,color)}

/* ********************************** */
/* draw_pixel functions with textures */
/* ********************************** */

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex_ztest_zset(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
			spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
		} \
	}
#else
	#define draw_pixel_tex_ztest_zset(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
			spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
		} \
	}
#endif

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex_ztest(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
		} \
	}
#else
	#define draw_pixel_tex_ztest(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
		} \
	}
#endif

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex_zset(x,y,z,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
		spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																					+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																						+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
		spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
	}
#else
	#define draw_pixel_tex_zset(x,y,z,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
		spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																					+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																						+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
		spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
	}
#endif

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex(x,y,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
		spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																					+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																						+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
	}
#else
	#define draw_pixel_tex(x,y,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
		spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																					+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																						+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
	}
#endif

// + Pattern
#define draw_pixel_tex_ztest_zset_pattern(x,y,z,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex_ztest_zset(x,y,z,u,v,color)}

#define draw_pixel_tex_ztest_pattern(x,y,z,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex_ztest(x,y,z,u,v,color)}

#define draw_pixel_tex_zset_pattern(x,y,z,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex_zset(x,y,z,u,v,color)}

#define draw_pixel_tex_pattern(x,y,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex(x,y,u,v,color)}

/* ******************************************** */
/* draw_pixel functions with textures and alpha */
/* ******************************************** */

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex_ztest_zset_alpha(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
			if (pixel != SP_ALPHA_COLOR) \
			{ \
				spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																							+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																								+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
				spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
			} \
		} \
	}
#else
	#define draw_pixel_tex_ztest_zset_alpha(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
			if (pixel != SP_ALPHA_COLOR) \
			{ \
				spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																							+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																								+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
				spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
			} \
		} \
	}
#endif

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex_ztest_alpha(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
			if (pixel != SP_ALPHA_COLOR) \
				spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																							+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																								+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
		} \
	}
#else
	#define draw_pixel_tex_ztest_alpha(x,y,z,u,v,color) \
	{ \
		if ( (z) < 0 && (z) > spZBuffer[(x) + (y) * spTargetScanLine] ) \
		{ \
			Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
			if (pixel != SP_ALPHA_COLOR) \
				spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																							+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																								+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
		} \
	}
#endif

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex_zset_alpha(x,y,z,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
		if (pixel != SP_ALPHA_COLOR) \
		{ \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
			spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
		} \
	}
#else
	#define draw_pixel_tex_zset_alpha(x,y,z,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
		if (pixel != SP_ALPHA_COLOR) \
		{ \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
			spZBuffer[(x) + (y) * spTargetScanLine] = (z); \
		} \
	}
#endif

#ifdef FAST_BUT_UGLY
	#define draw_pixel_tex_alpha(x,y,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(u) + (v) * spTextureScanLine];  \
		if (pixel != SP_ALPHA_COLOR) \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
	}
#else
	#define draw_pixel_tex_alpha(x,y,u,v,color) \
	{ \
		Uint32 pixel = spTexturePixel[(((u)<0)?0:((u)>=spTextureX)?spTextureX-1:(u)) + (((v)<0)?0:((v)>=spTextureY)?spTextureY-1:(v)) * spTextureScanLine];  \
		if (pixel != SP_ALPHA_COLOR) \
			spTargetPixel[(x) + (y) * spTargetScanLine] = ( ( pixel * (color) >> 16 ) & 63488 )  \
																						+ ( ( ( pixel & 2047 ) * ( (color) & 2047 ) >> 11 ) & 2016 )  \
																							+ ( ( pixel & 31 ) * ( (color) & 31 ) >> 5 ); \
	}
#endif

// + Pattern
#define draw_pixel_tex_ztest_zset_alpha_pattern(x,y,z,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex_ztest_zset_alpha(x,y,z,u,v,color)}

#define draw_pixel_tex_ztest_alpha_pattern(x,y,z,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex_ztest_alpha(x,y,z,u,v,color)}

#define draw_pixel_tex_zset_alpha_pattern(x,y,z,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex_zset_alpha(x,y,z,u,v,color)}

#define draw_pixel_tex_alpha_pattern(x,y,u,v,color) \
	{if ((spPattern[y & 7] >> (x & 7)) & 1) \
		draw_pixel_tex_alpha(x,y,u,v,color)}
