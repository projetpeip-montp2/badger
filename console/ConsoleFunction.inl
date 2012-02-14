////////////////////////////////////////////////////////////
// Copyright (c) 2011 - Hiairrassary Victor
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright 
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, this list of conditions and the following disclaimer in the 
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of its 
//       contributors may be used to endorse or promote products derived from 
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////


namespace plt
{
    namespace priv
    { 
        template <typename FuncType, typename ResultType>
        struct CallFunction
        {
            template<typename... ArgsType>
            inline static std::string Do(const std::function<FuncType> &function, ArgsType... args)
            {
                std::ostringstream oss;
                oss << function(args...);

	            return oss.str();
            }
        };


        template <typename FuncType>
        struct CallFunction<FuncType, void>
        {
            template<typename... ArgsType>
            inline static std::string Do(const std::function<FuncType> &function, ArgsType... args)
            {
                function(args...);

	            return std::string();
            }
        };






        template<typename FuncType, typename ToDo>
        struct ExecuteImpl;


        template<typename FuncType, template<typename...> class ToDo, typename ActualType, typename... ArgsAFaire>
        struct ExecuteImpl<FuncType, ToDo<ActualType, ArgsAFaire...>>
        {
            template<typename... ArgsDoneType>
            inline static std::string exec(StringExtractor& extractor, const std::function<FuncType> &function, ArgsDoneType... argsDone)
            {
                typename std::remove_const< typename std::remove_reference<ActualType>::type >::type actual;
                extractor >> actual;

                return ExecuteImpl<FuncType, ToDo<ArgsAFaire...> >::exec(extractor, function, argsDone..., actual);
            }
        };


        template<typename FuncType, template<typename...> class ToDo>
        struct ExecuteImpl<FuncType, ToDo<> >
        {
            template<typename... ArgsDoneType>
            inline static std::string exec(StringExtractor& extractor, const std::function<FuncType> &function, ArgsDoneType... argsDone)
            {
                extractor.throwIfEOF();
               
                return CallFunction<FuncType, typename std::function<FuncType>::result_type>::Do(function, argsDone...);
            }
        };

    } // namespace priv





    template<typename ResultType, typename... ArgsType>
    inline ConsoleFunction<ResultType, ArgsType...>::ConsoleFunction
    (
        const std::function<ResultType(ArgsType...)> &function
    ) :
    m_function(function)
    {

    }


    template<typename ResultType, typename... ArgsType>
    inline ConsoleFunction<ResultType, ArgsType...>::~ConsoleFunction
    (
    )
    {

    }


    template<typename ResultType, typename... ArgsType>
    inline std::string ConsoleFunction<ResultType, ArgsType...>::execute
    (
        const std::string &params
    )
    {
        StringExtractor extractor(params);

        return priv::ExecuteImpl<ResultType(ArgsType...), ToDo<ArgsType...>>::exec(extractor, m_function);
    }

} // namespace plt

