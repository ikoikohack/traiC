#include <stdio.h>

int main(int argc, char *argv[]){




	return 0;
}

void rapid_product(const Matrix* lhs, const Matrix* rhs, Matrix *result){


	if(lhs->cols != rhs->rows){
		fprintf(stderr,"Error\n");
	}
	result->rows = lhs->rows;
	result->lhs  = rhs->cols;
	Matrix transpose_rhs = *rhs;
	transpose(&transpose_rhs);

	for (int y = 0; y < result->rows;y++){
		for(int x = 0; x < result->cols; x++){
			int sum_element = 0;
			for (const register int *lhs_data = &lhs->data[y * lhs->cols],*rhs_data[x * rhs->cols];
				lhs_data < &lhs_data[(y + 1) * lhs->cols];
				++lhs_data, ++rhs_data){
			
				sum_element +=(*lhs_data) * (*rhs_data);

			}
			result->data[y * result->rows + x] = sum_element;

		}

	}



}
