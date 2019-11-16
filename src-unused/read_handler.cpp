//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

void
read_handler(std::string const& str)
{
  std::vector<std::string> messages = utl::parse(str, '\n');
  for (auto const& m : messages)
  {
    Msg::parse(m);
  }
}

void
read_handler(std::string const& str)
{
  using json = nlohmann::json;

  // Exceptions
  //  std::domain_error if a JSON value is not an object of the correct type.
  //  std::out_of_range if a specified key is not stored in j.
  try
  {
    // Accommodate multiple messages
    std::string s(str);
    utl::replace(s, "}\n{", "},{");

    // Parse JSON -- Throws an exception if an error occurred
    json j = json::parse("[" + s + "]");

    unsigned message_count = j.size();
    for (std::size_t i = 0; i != message_count; ++i)
    {
      eye::tracker::parse_message(j.at(i));
    }

  }
  catch (std::exception& e)
  {
    eye::debug::error(__FILE__, __LINE__, "exception: ", e.what());
  }
}

//===========================================================================//
