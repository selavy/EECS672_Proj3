#version 420 core

uniform int whichMountain;
in float heightFractionToFS;
out vec4 fragmentColor;

// for better control, these should be passed as uniforms:
vec4 tallMin = vec4(0.0, 0.0, 0.0, 1.0);
vec4 tallMid = vec4(0.3, 0.4, 0.3, 1.0);
vec4 tallMax = vec4(1.0, 1.0, 1.0, 1.0);
vec4 smallMin = vec4(0.1, 0.25, 0.1, 1.0);
vec4 smallMid = vec4(0.2, 0.45, 0.2, 1.0);
vec4 smallMax = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	if (whichMountain == 0)
	{
		// background tall mountain
		float midThreshhold = 0.65;
		if (heightFractionToFS <= midThreshhold)
			fragmentColor = mix(tallMin, tallMid, heightFractionToFS/midThreshhold);
		else
		{
			float f = (heightFractionToFS - midThreshhold) / (1.0 - midThreshhold);
			fragmentColor = mix(tallMid, tallMax, f);
		}
	}
	else
	{
		// foreground smaller mountain
		float midThreshhold = 0.75;
		if (heightFractionToFS <= midThreshhold)
			fragmentColor = mix(smallMin, smallMid, heightFractionToFS/midThreshhold);
		else
		{
			float f = (heightFractionToFS - midThreshhold) / (1.0 - midThreshhold);
			fragmentColor = mix(smallMid, smallMax, f);
		}
	}
}
