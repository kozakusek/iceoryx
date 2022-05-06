// Copyright (c) 2022 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef IOX_HOOFS_CLI_COMMAND_LINE_OPTION_SET_HPP
#define IOX_HOOFS_CLI_COMMAND_LINE_OPTION_SET_HPP

#include "iceoryx_hoofs/cli/types.hpp"
#include "iceoryx_hoofs/cxx/function.hpp"
#include "iceoryx_hoofs/cxx/vector.hpp"
#include "iceoryx_hoofs/internal/cli/command_line_option_value.hpp"
#include <cstdint>

namespace iox
{
namespace cli
{
namespace internal
{
/// @brief A set of options which is provided to the CommandLineArgumentParser.
///     Description, short and long name as well as type and value can be defined for every
///     command line option which the application provides.
///     The parser uses this set to populate the CommandLineOptionValue.
class CommandLineOptionSet
{
  public:
    static constexpr uint64_t MAX_TYPE_NAME_LENGTH = 16;
    static constexpr char NO_SHORT_OPTION = '\0';

    using TypeName_t = cxx::string<MAX_TYPE_NAME_LENGTH>;

    /// @brief The constructor.
    /// @param[in] programDescription The description to the program. Will be printed in the help.
    /// @param[in] onFailureCallback callback which is called when parse fails, if nothing is
    ///            defined std::exit(EXIT_FAILURE) is called
    explicit CommandLineOptionSet(
        const OptionDescription_t& programDescription,
        const cxx::function<void()> onFailureCallback = [] { std::exit(EXIT_FAILURE); }) noexcept;

    /// @brief Adds a command line switch argument
    ///        Calls the onFailureCallback when the option was already added or the shortOption and longOption are
    ///        empty.
    /// @param[in] shortOption a single letter as short option
    /// @param[in] longOption a multi letter word which does not start with minus as long option name
    /// @param[in] description the description to the argument
    CommandLineOptionSet&
    addSwitch(const char shortOption, const OptionName_t& longOption, const OptionDescription_t& description) noexcept;

    /// @brief Adds a command line optional value argument.
    ///        Calls the onFailureCallback when the option was already added or the shortOption and longOption are
    ///        empty.
    /// @param[in] shortOption a single letter as short option
    /// @param[in] longOption a multi letter word which does not start with minus as long option name
    /// @param[in] description the description to the argument
    /// @param[in] typeName the name of the value type
    /// @param[in] defaultValue the value which will be set to the option when it is not set by the user
    CommandLineOptionSet& addOptional(const char shortOption,
                                      const OptionName_t& longOption,
                                      const OptionDescription_t& description,
                                      const TypeName_t& typeName,
                                      const Argument_t& defaultValue) noexcept;

    /// @brief Adds a command line required value argument
    ///        Calls the onFailureCallback when the option was already added or the shortOption and longOption are
    ///        empty.
    /// @param[in] shortOption a single letter as short option
    /// @param[in] longOption a multi letter word which does not start with minus as long option name
    /// @param[in] description the description to the argument
    /// @param[in] typeName the name of the value type
    CommandLineOptionSet& addRequired(const char shortOption,
                                      const OptionName_t& longOption,
                                      const OptionDescription_t& description,
                                      const TypeName_t& typeName) noexcept;

  private:
    struct Value
    {
        char shortOption = NO_SHORT_OPTION;
        OptionName_t longOption;
        OptionDescription_t description;
        OptionType type = OptionType::SWITCH;
        TypeName_t typeName;
        Argument_t defaultValue;
    };

    friend class OptionManager;
    friend class CommandLineArgumentParser;
    friend std::ostream& operator<<(std::ostream&, const CommandLineOptionSet::Value&) noexcept;

    void sortAvailableOptions() noexcept;
    CommandLineOptionSet& addOption(const Value& option) noexcept;
    cxx::optional<Value> getOption(const OptionName_t& name) const noexcept;

  private:
    OptionDescription_t m_programDescription;
    cxx::vector<Value, CommandLineOptionValue::MAX_NUMBER_OF_ARGUMENTS> m_availableOptions;
    cxx::function<void()> m_onFailureCallback;
};

std::ostream& operator<<(std::ostream& stream, const CommandLineOptionSet::Value& value) noexcept;
} // namespace internal
} // namespace cli
} // namespace iox

#endif
