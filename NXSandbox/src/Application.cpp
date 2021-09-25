
namespace Neurex {
	__declspec(dllimport) void hello();
}

int main(int argc, char** argv) {
	Neurex::hello();
}