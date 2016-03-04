/* Package loader for R3X VM */

#include <system.h>
#include <r3x_pack.h>
package_info* load_package(char* package_name);

package_info* load_package(char* package_name) {
	(void)package_name;
	return NULL;
	/*package_info* pack_info = malloc(sizeof(package_info));
	package_info->package_file = fopen(package_file, "r");
	if(package_info->package_file == NULL) {
		return NULL;
	}
	void* temp = malloc(sizeof(r3x_package_header));
	fread(temp, sizeof(r3x_package_header), uint8_t, package_info->package_file);
	r3x_package_header* myheader = &temp;
	if(myheader->PackageHeader != R3X_PACKAGE_HEADER) {
		return NULL;
	}*/
}
