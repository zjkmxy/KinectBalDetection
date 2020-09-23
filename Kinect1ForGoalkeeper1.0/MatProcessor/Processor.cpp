#include "Processor.h"
#include "Converter.h"
#include "Whitener.h"
#include "Outputer.h"

IProcessor* IProcessor::GetDefaultProcessor()
{
	IProcessor* ret = nullptr;
	ret = new Converter(Converter::ColorSpace::CS_HSI);
	ret = new Whitener(false, ret);
	ret = new Outputer(ret);
	return ret;
}
