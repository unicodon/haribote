xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 83;
 -0.01503;-0.00433;-0.05033;,
 -0.01517;0.01601;-0.05033;,
 -0.01679;0.01729;-0.04583;,
 -0.01676;-0.00433;-0.04583;,
 -0.01734;0.01772;-0.04061;,
 -0.01728;-0.00433;-0.04061;,
 -0.01811;0.01630;-0.01397;,
 -0.01793;-0.00433;-0.01397;,
 -0.01756;0.01146;0.00019;,
 -0.01785;-0.00798;-0.00496;,
 -0.01691;0.00367;0.00804;,
 -0.01753;-0.00839;0.00295;,
 0.01772;-0.00886;0.00186;,
 0.01713;0.00395;0.00809;,
 0.01771;0.01146;0.00019;,
 0.01791;-0.00798;-0.00496;,
 0.01836;0.01630;-0.01397;,
 0.01796;-0.00470;-0.01397;,
 0.01738;0.01772;-0.04061;,
 0.01721;-0.00433;-0.04061;,
 0.01691;0.01734;-0.04583;,
 0.01686;-0.00433;-0.04578;,
 0.01548;0.01601;-0.05033;,
 0.01548;-0.00433;-0.05033;,
 0.00955;0.01513;-0.05502;,
 0.00955;-0.00433;-0.05477;,
 -0.00964;-0.00433;-0.05472;,
 -0.00964;0.01497;-0.05473;,
 -0.00800;0.02101;-0.04733;,
 0.00749;0.02119;-0.04761;,
 0.01199;0.02208;-0.04078;,
 -0.01175;0.02216;-0.04078;,
 0.01260;0.02064;-0.01397;,
 -0.01270;0.02086;-0.01397;,
 0.01261;0.01637;0.00019;,
 -0.01231;0.01644;0.00019;,
 -0.01160;0.01038;0.00878;,
 0.01194;0.01067;0.00838;,
 0.01153;0.00453;0.01261;,
 -0.01135;0.00457;0.01259;,
 -0.01517;0.01601;-0.05033;,
 -0.01109;0.01896;-0.04956;,
 -0.01360;0.01971;-0.04583;,
 -0.01679;0.01729;-0.04583;,
 -0.01478;0.02016;-0.04061;,
 -0.01734;0.01772;-0.04061;,
 -0.01560;0.01904;-0.01397;,
 -0.01811;0.01630;-0.01397;,
 -0.01555;0.01495;0.00019;,
 -0.01756;0.01146;0.00019;,
 -0.01509;0.00835;0.00923;,
 -0.01691;0.00367;0.00804;,
 0.01713;0.00395;0.00809;,
 0.01518;0.00813;0.00923;,
 0.01564;0.01467;0.00038;,
 0.01771;0.01146;0.00019;,
 0.01584;0.01900;-0.01392;,
 0.01836;0.01630;-0.01397;,
 0.01493;0.02013;-0.04064;,
 0.01738;0.01772;-0.04061;,
 0.01385;0.01959;-0.04573;,
 0.01691;0.01734;-0.04583;,
 0.01080;0.01887;-0.05001;,
 0.01548;0.01601;-0.05033;,
 0.00649;0.01836;-0.05238;,
 0.00955;0.01513;-0.05502;,
 -0.00804;0.01845;-0.05177;,
 -0.00964;0.01497;-0.05473;,
 0.00749;0.02119;-0.04761;,
 -0.00800;0.02101;-0.04733;,
 0.01199;0.02208;-0.04078;,
 -0.01175;0.02216;-0.04078;,
 0.01260;0.02064;-0.01397;,
 -0.01270;0.02086;-0.01397;,
 0.01261;0.01637;0.00019;,
 -0.01231;0.01644;0.00019;,
 0.01506;0.00457;0.01146;,
 -0.01488;0.00445;0.01150;,
 -0.01489;0.00084;0.01234;,
 -0.01753;-0.00839;0.00295;,
 -0.01691;0.00367;0.00804;,
 -0.01489;0.00084;0.01234;,
 -0.01541;-0.00897;0.00867;;
 
 47;
 4;3,2,1,0;,
 4;5,4,2,3;,
 4;7,6,4,5;,
 4;9,8,6,7;,
 4;11,10,8,9;,
 4;15,14,13,12;,
 4;17,16,14,15;,
 4;19,18,16,17;,
 4;21,20,18,19;,
 4;23,22,20,21;,
 4;25,24,22,23;,
 4;0,1,27,26;,
 4;31,30,29,28;,
 4;33,32,30,31;,
 4;35,34,32,33;,
 4;39,38,37,36;,
 4;36,37,34,35;,
 4;43,42,41,40;,
 4;45,44,42,43;,
 4;47,46,44,45;,
 4;49,48,46,47;,
 4;51,50,48,49;,
 4;55,54,53,52;,
 4;57,56,54,55;,
 4;59,58,56,57;,
 4;61,60,58,59;,
 4;63,62,60,61;,
 4;65,64,62,63;,
 4;67,66,64,65;,
 4;40,41,66,67;,
 4;68,60,62,64;,
 4;69,68,64,66;,
 4;42,69,66,41;,
 4;70,58,60,68;,
 4;44,71,69,42;,
 4;72,56,58,70;,
 4;46,73,71,44;,
 4;74,54,56,72;,
 4;48,75,73,46;,
 4;38,76,53,37;,
 4;50,77,39,36;,
 4;37,53,54,74;,
 4;50,36,75,48;,
 3;78,77,51;,
 3;52,53,76;,
 3;77,50,51;,
 4;82,81,80,79;;
 
 MeshMaterialList {
  2;
  47;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0;;
  Material {
   0.267000;0.267000;0.267000;1.000000;;
   5.000000;
   0.405000;0.405000;0.405000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.784000;0.784000;0.784000;1.000000;;
   5.000000;
   0.330000;0.330000;0.330000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  62;
  -0.811338;-0.003312;-0.584567;,
  -0.973944;-0.003066;-0.226768;,
  -0.997937;-0.003918;-0.064087;,
  -0.999997;-0.002225;-0.001167;,
  -0.999087;0.007682;0.042037;,
  -0.998123;0.014058;0.059603;,
  0.998730;0.011609;0.049026;,
  0.999324;0.003193;0.036618;,
  0.999949;-0.009143;-0.004291;,
  0.998420;-0.008960;-0.055481;,
  0.982086;-0.003124;-0.188410;,
  0.820852;-0.003390;-0.571132;,
  0.609842;-0.005116;-0.792506;,
  -0.627348;-0.002381;-0.778736;,
  -0.119038;0.534310;0.836865;,
  0.122350;0.545083;0.829406;,
  -0.005463;0.987743;-0.155995;,
  0.000248;0.998609;-0.052718;,
  0.005690;0.984803;0.173582;,
  0.003148;0.898561;0.438837;,
  0.166511;0.679147;0.714866;,
  -0.151619;0.677124;0.720080;,
  -0.379206;0.774206;-0.506762;,
  -0.435892;0.721894;-0.537464;,
  -0.466610;0.820325;-0.330670;,
  -0.579714;0.756103;-0.303710;,
  -0.589296;0.801612;-0.100742;,
  -0.679892;0.729214;-0.077417;,
  -0.639406;0.758503;0.125831;,
  -0.757879;0.643784;0.105647;,
  -0.647763;0.669401;0.363737;,
  -0.832790;0.476558;0.281696;,
  -0.627161;0.497824;0.599033;,
  -0.868175;0.235210;0.436976;,
  0.620145;0.513626;0.592966;,
  0.841336;0.333030;0.425728;,
  0.641919;0.673168;0.367130;,
  0.812139;0.500602;0.299713;,
  0.624417;0.770082;0.130675;,
  0.750984;0.650768;0.111910;,
  0.583226;0.806179;-0.099611;,
  0.680605;0.728631;-0.076633;,
  0.465694;0.820175;-0.332328;,
  0.570896;0.766686;-0.293718;,
  0.360538;0.785394;-0.503159;,
  0.407337;0.748851;-0.522781;,
  0.138574;0.787073;-0.601094;,
  0.150129;0.692822;-0.705308;,
  -0.168200;0.782499;-0.599504;,
  -0.193234;0.674988;-0.712075;,
  0.233391;0.877335;-0.419300;,
  -0.239278;0.880803;-0.408573;,
  0.475743;0.871203;-0.121133;,
  -0.489748;0.863172;-0.122803;,
  0.478713;0.865841;0.145442;,
  -0.503093;0.852393;0.142561;,
  0.430130;0.803559;0.411438;,
  -0.410970;0.810421;0.417519;,
  -0.867151;0.116256;0.484286;,
  -0.708526;0.301534;0.638019;,
  0.582484;0.425795;0.692395;,
  -0.930087;-0.092431;0.355519;;
  47;
  4;1,1,0,0;,
  4;2,2,1,1;,
  4;3,3,2,2;,
  4;4,4,3,3;,
  4;5,5,4,4;,
  4;7,7,6,6;,
  4;8,8,7,7;,
  4;9,9,8,8;,
  4;10,10,9,9;,
  4;11,11,10,10;,
  4;12,12,11,11;,
  4;0,0,13,13;,
  4;17,17,16,16;,
  4;18,18,17,17;,
  4;19,19,18,18;,
  4;14,15,20,21;,
  4;21,20,19,19;,
  4;25,24,22,23;,
  4;27,26,24,25;,
  4;29,28,26,27;,
  4;31,30,28,29;,
  4;33,32,30,31;,
  4;37,36,34,35;,
  4;39,38,36,37;,
  4;41,40,38,39;,
  4;43,42,40,41;,
  4;45,44,42,43;,
  4;47,46,44,45;,
  4;49,48,46,47;,
  4;23,22,48,49;,
  4;50,42,44,46;,
  4;51,50,46,48;,
  4;24,51,48,22;,
  4;52,40,42,50;,
  4;26,53,51,24;,
  4;54,38,40,52;,
  4;28,55,53,26;,
  4;56,36,38,54;,
  4;30,57,55,28;,
  4;15,60,34,20;,
  4;32,59,14,21;,
  4;20,34,36,56;,
  4;32,21,57,30;,
  3;58,59,33;,
  3;35,34,60;,
  3;59,32,33;,
  4;61,61,61,61;;
 }
}
