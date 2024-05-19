#pragma once
/*=============================================================================
    Copyright (c) 2023 Larry Joe Evans

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
namespace boost { namespace spirit { namespace x3
{
    template <typename ID>
    struct rule_internal_id
    //Purpose:
    //  Create a tag for transform_attribute_tagged
    //  for transforming attribute of rhs of rule with id, ID
    //  within that rule's parse function.
    {};
}}}
