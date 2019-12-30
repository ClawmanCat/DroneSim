__kernel void main(size_t kcount, float c, __global float* x, __global float* y, __global float* output) {
	int i = get_global_id(0);
	output[i] = c * (x[i] + y[i]);
}