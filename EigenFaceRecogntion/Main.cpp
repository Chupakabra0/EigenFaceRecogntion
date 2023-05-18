#include "App.hpp"
#include "TestCasesImagesPathGetter.hpp"
#include "DefaultImageReader.hpp"
#include "GrayscaleImageProcessor.hpp"
#include "ResizingImageProcessor.hpp"
#include "DefaultArgumentsParser.hpp"
#include "DefaultAppLogger.hpp"
#include "SilenceLogger.hpp"
#include "ImageLogger.hpp"
#include "JSONService.hpp"
#include "Consts.hpp"

int main(int argc, char** argv) {
	IArgumentsParser* argumentsParser(new DefaultArgumentsParser("It's FUCKIN' description"));

	if (!argumentsParser->ParseAll(argc, argv)) {
		return 0;
	}

	IImageReader* imageReader(new DefaultImageReader());
	IImageProcessor* imageProcessorChain(
		new GrayscaleImageProcessor(new ResizingImageProcessor(BASIS_IMAGE_HEIGHT, BASIS_IMAGE_WIDTH))
	);
	IAppLogger* appLogger(new SilenceLogger());
	IFileService* fileService(new JSONService());

	App app(imageReader, imageProcessorChain, appLogger, argumentsParser, fileService);

	return app.ExecuteApp(argc, argv);
}
