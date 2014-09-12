uniform sampler2D texture;

uniform float blurSize;
uniform float horizontalPass;
uniform float sigma;
uniform float width;
uniform float height;

vec2 ranged(vec2 v)
{
	if (v.x < 0.0)
		v.x += 1.0;
	else if (v.x >= 1.0)
		v.x -= 1.0;

	if (v.y < 0.0)
		v.y += 1.0;
	else if (v.y >= 1.0)
		v.y -= 1.0;

	return v;
}

void main()
{
	vec2 texcoordOffset = vec2(1.0, 1.0) / vec2(width, height);

	float k0 = 1.0;
	float k1 = 0.5;
	float k2 = 0.357;

	vec4 avgValue =
		texture2D(texture, gl_TexCoord[0].st) * k0 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(-1.0, -1.0))) * k1 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(1.0, -1.0))) * k1 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(-1.0, 1.0))) * k1 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(1.0, 1.0))) * k1 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(-1.0, 0.0))) * k2 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(1.0, 0.0))) * k2 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(0.0, -1.0))) * k2 +
		texture2D(texture, ranged(gl_TexCoord[0].st - texcoordOffset * vec2(0.0, 1.0))) * k2;

	gl_FragColor = avgValue / (k0 + 4.0 * k1 + 4.0 * k2);
}