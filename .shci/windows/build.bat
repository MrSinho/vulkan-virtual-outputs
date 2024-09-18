> .shci/windows/build-output.txt (
	
    mkdir build
    cd    build
	mkdir windows
	cd    windows

	cmake ../../ -DSH_TEMPLATE_BUILD_EXAMPLE=ON
	cmake --build .
)
