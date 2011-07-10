#version 120
#extension GL_ARB_draw_buffers: enable
uniform float width, height;
uniform sampler2D rand;
uniform float d;
uniform float dScale;
uniform float xyScale;






int wrap32(float f) {
  int i = int(f);
  i -= int(i / 32) * 32;
  return i;
}

float wrap1f(float f) {
  return f - floor(f);
}

vec4 quad_dot(vec4 a, vec4 b, vec4 c, vec4 d) {
  return vec4(dot(a, a), dot(b, b), dot(c, c), dot(d, d));
}

vec4 grad_select(int i) {
  int id = i / 8;
  vec4 grad;
  grad.x = 0;
  grad.y = (mod(i / 4, 2) == 0) ? 1 : -1;
  grad.z = (mod(i / 2, 2) == 0) ? 1 : -1;
  grad.w = (mod(i, 2) == 0) ? 1 : -1;
  if (id < 2)
    if (id == 0) return grad.xyzw;
    else return grad.yxzw;
  else
    if (id == 2) return grad.yzxw;
    else return grad.yzwx;
}

float perm(float f) {
  return texture2D(rand, vec2(0.5, f)).a;
}

float simpnoise4(vec4 v) {
  const float F = (sqrt(5.0) - 1.0) / 4.0;
  const float G = (5.0 - sqrt(5.0)) / 20.0;
  float s = (v.x + v.y + v.z + v.w) * F;
  vec4 floors = floor(v + vec4(s));
  float t = (floors.x + floors.y + floors.z + floors.w) * G;
  
  vec4 v0 = v - floors + vec4(t);
  
  // xyzw
  // yzwx -> cmp1
  // zwxy -> cmp2
  // wxyz -> cmp3
  
  bvec4 cmp1 = greaterThan(v0, v0.yzwx);
  bvec4 cmp2 = greaterThan(v0, v0.zwxy);
  bvec4 cmp3 = greaterThanEqual(v0, v0.wxyz); // do not optimize this away. it is faster like this.
  ivec4 si = ivec4(cmp1) + ivec4(cmp2) + ivec4(cmp3);
  /*
  vec4[3] greatf = vec4[3](
    vec4(greaterThan(si, ivec4(2))),
    vec4(greaterThan(si, ivec4(1))),
    vec4(greaterThan(si, ivec4(0)))
  );
  */
  vec4[3] greatf;
  greatf[0] = vec4(greaterThan(si, ivec4(2)));
  greatf[1] = vec4(greaterThan(si, ivec4(1)));
  greatf[2] = vec4(greaterThan(si, ivec4(0)));
  
  vec4 v1 = v0 - greatf[0] + vec4(G);
  vec4 v2 = v0 - greatf[1] + vec4(2.0 * G);
  vec4 v3 = v0 - greatf[2] + vec4(3.0 * G);
  vec4 v4 = v0             + vec4(4.0 * G - 1.0);
  greatf[0] /= 256.0;
  greatf[1] /= 256.0;
  greatf[2] /= 256.0;
  floors /= 256.0;
  float ii = wrap1f(floors.x), jj = wrap1f(floors.y), kk = wrap1f(floors.z), ll = wrap1f(floors.w);
  int gi0 = wrap32(perm(ii            +perm(jj            +perm(kk            +perm(ll            ))))*256.0);
  int gi1 = wrap32(perm(ii+greatf[0].x+perm(jj+greatf[0].y+perm(kk+greatf[0].z+perm(ll+greatf[0].w))))*256.0);
  int gi2 = wrap32(perm(ii+greatf[1].x+perm(jj+greatf[1].y+perm(kk+greatf[1].z+perm(ll+greatf[1].w))))*256.0);
  int gi3 = wrap32(perm(ii+greatf[2].x+perm(jj+greatf[2].y+perm(kk+greatf[2].z+perm(ll+greatf[2].w))))*256.0);
  int gi4 = wrap32(perm(ii+(1.0/256.0)+perm(jj+(1.0/256.0)+perm(kk+(1.0/256.0)+perm(ll+(1.0/256.0)))))*256.0);
  
  float contrib_sum = 0.0;
  float cc0 = 0.6 - dot(v0, v0);
  if (cc0 > 0.0) { cc0 *= cc0; contrib_sum += cc0 * cc0 * dot(v0, grad_select(gi0)); }
  float cc1 = 0.6 - dot(v1, v1);
  if (cc1 > 0.0) { cc1 *= cc1; contrib_sum += cc1 * cc1 * dot(v1, grad_select(gi1)); }
  float cc2 = 0.6 - dot(v2, v2);
  if (cc2 > 0.0) { cc2 *= cc2; contrib_sum += cc2 * cc2 * dot(v2, grad_select(gi2)); }
  float cc3 = 0.6 - dot(v3, v3);
  if (cc3 > 0.0) { cc3 *= cc3; contrib_sum += cc3 * cc3 * dot(v3, grad_select(gi3)); }
  float cc4 = 0.6 - dot(v4, v4);
  if (cc4 > 0.0) { cc4 *= cc4; contrib_sum += cc4 * cc4 * dot(v4, grad_select(gi4)); }
  
  return 0.5 + 13.5 * contrib_sum;
}

float perlin3(vec2 tc, float ds, int offs) {
  return simpnoise4(vec4(tc * 1, ds * 1, offs+0)) * 0.5
       + simpnoise4(vec4(tc * 2, ds * 1, offs+1)) * 0.25;
//       + simpnoise4(vec4(tc * 4, ds * 1, offs+2)) * 0.125
//       + simpnoise4(vec4(tc * 8, ds * 1, offs+3)) * 0.0625;
}

vec3 perlin3test(vec2 tc, float ds, int offs) {
  return vec3(
    simpnoise4(vec4(tc * 1, ds * 6, offs+0)),
    simpnoise4(vec4(tc * 1, ds * 6, offs+1)),
    simpnoise4(vec4(tc * 1, ds * 6, offs+2))
  ) * 0.125
  + vec3(simpnoise4(vec4(tc * 2, ds * 3, offs+3))) * 0.5
  + vec3(simpnoise4(vec4(tc * 4, ds * 2, offs+4))) * 0.25
  + vec3(simpnoise4(vec4(tc * 8, ds * 1, offs+3))) * 0.125;
}




















void main(void) 
{
vec2 tc = gl_TexCoord[0].st * 0.005 * xyScale;
  tc.x += d*0.1;
  float ds = d * dScale;
  gl_FragColor = vec4(simpnoise4(vec4(tc.x, tc.y, ds, 0)), simpnoise4(vec4(tc.x, tc.y, ds, 1)), simpnoise4(vec4(tc.x, tc.y, ds, 2)), 1.f);
  // make it signed noise
	gl_FragColor[0]-= 0.5;
	gl_FragColor[1]-= 0.5;
	gl_FragColor[2]-= 0.5;
	gl_FragColor[3]-= 0.5;

	
//  gl_FragColor = vec4(perlin3(tc, ds, 0), perlin3(tc, ds, 10), perlin3(tc, ds, 20), 1);
//	gl_FragColor = vec4(0, 0, 0, 0);

}
