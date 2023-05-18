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
	IAppLogger* appLogger(new ImageLogger());
	IFileService* fileService(new JSONService());

	App app(imageReader, imageProcessorChain, appLogger, argumentsParser, fileService);

	return app.ExecuteApp(argc, argv);
}
//
//#include <iostream>
//#include <args/args.hpp>
//
//int main(int argc, char** argv)
//{
//    args::ArgumentParser parser("This is a test program.", "This goes after the options.");
//    args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
//    args::ValueFlag<int> integer(parser, "integer", "The integer flag", { 'i' });
//    args::ValueFlagList<char> characters(parser, "characters", "The character flag", { 'c' });
//    args::Positional<std::string> foo(parser, "foo", "The foo position");
//    args::PositionalList<double> numbers(parser, "numbers", "The numbers position list", { 'n' });
//    try
//    {
//        parser.ParseCLI(argc, argv);
//    }
//    catch (args::Help)
//    {
//        std::cout << parser;
//        return 0;
//    }
//    catch (args::ParseError e)
//    {
//        std::cerr << e.what() << std::endl;
//        std::cerr << parser;
//        return 1;
//    }
//    catch (args::ValidationError e)
//    {
//        std::cerr << e.what() << std::endl;
//        std::cerr << parser;
//        return 1;
//    }
//    if (integer) { std::cout << "i: " << args::get(integer) << std::endl; }
//    if (characters) { for (const auto ch : args::get(characters)) { std::cout << "c: " << ch << std::endl; } }
//    if (foo) { std::cout << "f: " << args::get(foo) << std::endl; }
//    if (numbers) { for (const auto nm : args::get(numbers)) { std::cout << "n: " << nm << std::endl; } }
//    return 0;
//}
