#include <cctype>

#include "blockyscalarparser.hpp"

int BlockyScalarParser::try_parse
(
    const char* buffer,
    int offset,
    int count
)
{
    // REGEX: [+-]\d+\.?\d*[eE]*[+-]*\d*\s
    int checked = 0;

    if (count < 1)
        return TRY_PARSE_BUFFER_SHORT;

    std::string number;

	if (buffer[offset + checked] == '+')
	{
		number += '+';
		checked++;

		if (checked >= count)
			return TRY_PARSE_BUFFER_SHORT;
	}
	else if (buffer[offset + checked] == '-')
	{
		number += '-';
		checked++;

		if (checked >= count)
			return TRY_PARSE_BUFFER_SHORT;
	}

	if (!isdigit(buffer[offset + checked]))
		return TRY_PARSE_INVALID;
    
	while(isdigit(buffer[offset + checked]))
    {
        number += buffer[offset + checked];
        checked++;

        if (checked >= count)
            return TRY_PARSE_BUFFER_SHORT;
    }

    if (buffer[offset + checked] != '.')
        if (!isspace(buffer[offset + checked]))
            return TRY_PARSE_INVALID;

    if (buffer[offset + checked] == '.')
    {
        number += '.';
        checked++;

        if (checked >= count)
            return TRY_PARSE_BUFFER_SHORT;

        while
		(
			isdigit(buffer[offset + checked]) 
		 || buffer[offset + checked] == 'e'
		 || buffer[offset + checked] == 'E'
		 || buffer[offset + checked] == '+'
		 || buffer[offset + checked] == '-'
		)
        {
            number += buffer[offset + checked];
            checked++;

            if (checked >= count)
                return TRY_PARSE_BUFFER_SHORT;
        }
    }

    if (isspace(buffer[offset + checked]) || buffer[offset + checked] == ')')
    {
        // TODO: validate, if number really is blockynumber
        // but should be true

        return checked;
    }

    return TRY_PARSE_INVALID;
}

int BlockyScalarParser::parse_constant
(
    const char* buffer,
    int offset,
    int count
)
{
    int parsed = 0;

    if (count < 1)
        // TODO: return unexpected end of buffer
        return -1;

    while
    (
        count > parsed
     && !isspace(buffer[offset + parsed])
	 && buffer[offset + parsed] != ')'
    )
        parsed++;

    if (parsed >= count)
        // TODO: return unexpected end of buffer
        return -1;

    std::string number(buffer + offset, parsed);
    //std::cout << "number: " << number << '\n';

    hooker.handle_list_entry(number);

    return parsed;
}
