# 说明

基于 [手把手教你构建 C 语言编译器](https://lotabout.me/2015/write-a-C-interpreter-0/) 写的 demo程序

# 参考

## C Operator Precedence

<table class="wikitable">
  <tbody>
    <tr>
      <th style="text-align: left">Precedence</th>
      <th style="text-align: left">Operator</th>
      <th style="text-align: left">Description</th>
      <th style="text-align: left">Associativity</th>
    </tr>
    <tr>
      <th>1</th>
      <td><code>::</code></td>
      <td>
        <a
          href="/w/cpp/language/identifiers#Qualified_identifiers"
          title="cpp/language/identifiers"
          >Scope resolution</a
        >
      </td>
      <td style="vertical-align: top" rowspan="6">Left-to-right →</td>
    </tr>
    <tr>
      <th rowspan="5">2</th>
      <td style="border-bottom-style: none">
        <code>a++</code> &nbsp; <code>a--</code>
      </td>
      <td style="border-bottom-style: none">
        Suffix/postfix
        <a
          href="/w/cpp/language/operator_incdec"
          title="cpp/language/operator incdec"
          >increment and decrement</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code><i>type</i>()</code> &nbsp; <code><i>type</i>{} </code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/explicit_cast"
          title="cpp/language/explicit cast"
          >Functional cast</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>a()</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_other#Built-in_function_call_operator"
          title="cpp/language/operator other"
          >Function call</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>a[]</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_member_access#Built-in_subscript_operator"
          title="cpp/language/operator member access"
          >Subscript</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>.</code> &nbsp; <code>-&gt;</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_member_access#Built-in_member_access_operators"
          title="cpp/language/operator member access"
          >Member access</a
        >
      </td>
    </tr>
    <tr>
      <th rowspan="10">3</th>
      <td style="border-bottom-style: none">
        <code>++a</code> &nbsp; <code>--a</code>
      </td>
      <td style="border-bottom-style: none">
        Prefix
        <a
          href="/w/cpp/language/operator_incdec"
          title="cpp/language/operator incdec"
          >increment and decrement</a
        >
      </td>
      <td style="vertical-align: top" rowspan="10">Right-to-left ←</td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>+a</code> &nbsp; <code>-a</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        Unary
        <a
          href="/w/cpp/language/operator_arithmetic#Unary_arithmetic_operators"
          title="cpp/language/operator arithmetic"
          >plus and minus</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>!</code> &nbsp; <code>~</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_logical"
          title="cpp/language/operator logical"
          >Logical NOT</a
        >
        and
        <a
          href="/w/cpp/language/operator_arithmetic#Bitwise_logic_operators"
          title="cpp/language/operator arithmetic"
          >bitwise NOT</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>(<i>type</i>)</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/explicit_cast"
          title="cpp/language/explicit cast"
          >C-style cast</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>*a</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_member_access#Built-in_indirection_operator"
          title="cpp/language/operator member access"
          >Indirection</a
        >
        (dereference)
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>&amp;a</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_member_access#Built-in_address-of_operator"
          title="cpp/language/operator member access"
          >Address-of</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/sizeof" title="cpp/language/sizeof"
          ><code>sizeof</code></a
        >
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/sizeof" title="cpp/language/sizeof">Size-of</a
        ><sup id="cite_ref-1" class="reference"
          ><a href="#cite_note-1">[note 1]</a></sup
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/keyword/co_await" title="cpp/keyword/co await"
          ><tt>co_await</tt></a
        >
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/coroutines" title="cpp/language/coroutines"
          >await-expression</a
        >
        <span class="t-mark-rev t-since-cxx20">(C++20)</span>
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/new" title="cpp/language/new"
          ><code>new</code></a
        >
        &nbsp;
        <a href="/w/cpp/language/new" title="cpp/language/new"
          ><code>new[]</code></a
        >
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/new" title="cpp/language/new"
          >Dynamic memory allocation</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-top-style: none">
        <a href="/w/cpp/language/delete" title="cpp/language/delete"
          ><code>delete</code></a
        >
        &nbsp;
        <a href="/w/cpp/language/delete" title="cpp/language/delete"
          ><code>delete[]</code></a
        >
      </td>
      <td style="border-top-style: none">
        <a href="/w/cpp/language/delete" title="cpp/language/delete"
          >Dynamic memory deallocation</a
        >
      </td>
    </tr>
    <tr>
      <th>4</th>
      <td><code>.*</code> &nbsp; <code>-&gt;*</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_member_access#Built-in_pointer-to-member_access_operators"
          title="cpp/language/operator member access"
          >Pointer-to-member</a
        >
      </td>
      <td style="vertical-align: top" rowspan="12">Left-to-right →</td>
    </tr>
    <tr>
      <th>5</th>
      <td><code>a*b</code> &nbsp; <code>a/b</code> &nbsp; <code>a%b</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_arithmetic#Multiplicative_operators"
          title="cpp/language/operator arithmetic"
          >Multiplication, division, and remainder</a
        >
      </td>
    </tr>
    <tr>
      <th>6</th>
      <td><code>a+b</code> &nbsp; <code>a-b</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_arithmetic#Additive_operators"
          title="cpp/language/operator arithmetic"
          >Addition and subtraction</a
        >
      </td>
    </tr>
    <tr>
      <th>7</th>
      <td><code>&lt;&lt;</code> &nbsp; <code>&gt;&gt;</code></td>
      <td>
        Bitwise
        <a
          href="/w/cpp/language/operator_arithmetic#Bitwise_shift_operators"
          title="cpp/language/operator arithmetic"
          >left shift and right shift</a
        >
      </td>
    </tr>
    <tr>
      <th>8</th>
      <td><code>&lt;=&gt;</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_comparison#Three-way_comparison"
          title="cpp/language/operator comparison"
          >Three-way comparison operator</a
        >
        <span class="t-mark-rev t-since-cxx20">(since C++20)</span>
      </td>
    </tr>
    <tr>
      <th>9</th>
      <td>
        <code>&lt;</code> &nbsp; <code>&lt;=</code> &nbsp;
        <code>&gt;</code> &nbsp; <code>&gt;=</code>
      </td>
      <td>
        For
        <a
          href="/w/cpp/language/operator_comparison"
          title="cpp/language/operator comparison"
          >relational operators</a
        >
        &lt; and ≤ and &gt; and ≥ respectively
      </td>
    </tr>
    <tr>
      <th>10</th>
      <td><code>==</code> &nbsp; <code>!=</code></td>
      <td>
        For
        <a
          href="/w/cpp/language/operator_comparison"
          title="cpp/language/operator comparison"
          >equality operators</a
        >
        = and ≠ respectively
      </td>
    </tr>
    <tr>
      <th>11</th>
      <td><code>a&amp;b</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_arithmetic#Bitwise_logic_operators"
          title="cpp/language/operator arithmetic"
          >Bitwise AND</a
        >
      </td>
    </tr>
    <tr>
      <th>12</th>
      <td><code>^</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_arithmetic#Bitwise_logic_operators"
          title="cpp/language/operator arithmetic"
          >Bitwise XOR</a
        >
        (exclusive or)
      </td>
    </tr>
    <tr>
      <th>13</th>
      <td><code>|</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_arithmetic#Bitwise_logic_operators"
          title="cpp/language/operator arithmetic"
          >Bitwise OR</a
        >
        (inclusive or)
      </td>
    </tr>
    <tr>
      <th>14</th>
      <td><code>&amp;&amp;</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_logical"
          title="cpp/language/operator logical"
          >Logical AND</a
        >
      </td>
    </tr>
    <tr>
      <th>15</th>
      <td><code>||</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_logical"
          title="cpp/language/operator logical"
          >Logical OR</a
        >
      </td>
    </tr>
    <tr>
      <th rowspan="8">16</th>
      <td style="border-bottom-style: none"><code>a?b:c</code></td>
      <td style="border-bottom-style: none">
        <a
          href="/w/cpp/language/operator_other#Conditional_operator"
          title="cpp/language/operator other"
          >Ternary conditional</a
        ><sup id="cite_ref-2" class="reference"
          ><a href="#cite_note-2">[note 2]</a></sup
        >
      </td>
      <td style="vertical-align: top" rowspan="8">Right-to-left ←</td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/throw" title="cpp/language/throw"
          ><code>throw</code></a
        >
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/throw" title="cpp/language/throw"
          >throw operator</a
        >
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/keyword/co_yield" title="cpp/keyword/co yield"
          ><tt>co_yield</tt></a
        >
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a href="/w/cpp/language/coroutines" title="cpp/language/coroutines"
          >yield-expression</a
        >
        <span class="t-mark-rev t-since-cxx20">(C++20)</span>
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>=</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_assignment#Builtin_direct_assignment"
          title="cpp/language/operator assignment"
          >Direct assignment</a
        >
        (provided by default for C++ classes)
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>+=</code> &nbsp; <code>-=</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_assignment#Builtin_compound_assignment"
          title="cpp/language/operator assignment"
          >Compound assignment</a
        >
        by sum and difference
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>*=</code> &nbsp; <code>/=</code> &nbsp; <code>%=</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_assignment#Builtin_compound_assignment"
          title="cpp/language/operator assignment"
          >Compound assignment</a
        >
        by product, quotient, and remainder
      </td>
    </tr>
    <tr>
      <td style="border-bottom-style: none; border-top-style: none">
        <code>&lt;&lt;=</code> &nbsp; <code>&gt;&gt;=</code>
      </td>
      <td style="border-bottom-style: none; border-top-style: none">
        <a
          href="/w/cpp/language/operator_assignment#Builtin_compound_assignment"
          title="cpp/language/operator assignment"
          >Compound assignment</a
        >
        by bitwise left shift and right shift
      </td>
    </tr>
    <tr>
      <td style="border-top-style: none">
        <code>&amp;=</code> &nbsp; <code>^=</code> &nbsp; <code>|=</code>
      </td>
      <td style="border-top-style: none">
        <a
          href="/w/cpp/language/operator_assignment#Builtin_compound_assignment"
          title="cpp/language/operator assignment"
          >Compound assignment</a
        >
        by bitwise AND, XOR, and OR
      </td>
    </tr>
    <tr>
      <th>17</th>
      <td><code>,</code></td>
      <td>
        <a
          href="/w/cpp/language/operator_other#Built-in_comma_operator"
          title="cpp/language/operator other"
          >Comma</a
        >
      </td>
      <td>Left-to-right →</td>
    </tr>
  </tbody>
</table>
