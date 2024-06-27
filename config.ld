project = 'FTSE'
description = 'Fallout Tactics Scripting Engine'
full_description = ''
title = 'FTSE Documentation'
dir = 'docs'
style = '!fixed'
use_markdown_titles = true
topics = 'doc_source/manual'
examples = {'doc_source/examples'}
package = 'ftse'
format = 'discount'
sort_modules=true
file = 'doc_source/ftse'
kind_names={topic='Manual',module='Hooks',classmod='Global Classes'}
tparam_alias('array','array')
tparam_alias('array2d','array')
alias('ret',{'return',modifiers={type="$1"}})
new_type("entity", "Entity classes", true)
new_type("vtable", "Vtable functions", true)
