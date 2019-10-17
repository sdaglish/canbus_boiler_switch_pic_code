syntax on

set tabstop=4 
set shiftwidth=4 
set expandtab
set autoindent

set colorcolumn=80 
highlight ColorColumn ctermbg=darkgray

augroup project   
    autocmd!   
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen 
augroup END

let g:syntastic_c_checkers = ['make']
let g:syntastic_c_check_header = 1
let g:syntastic_c_include_dirs = ['"C:/Program Files (x86)/Microchip/xc16/v1.41/support/generic/h"']
let g:syntastic_auto_loc_list=1
let g:syntastic_check_on_open=1
let g:syntastic_enable_signs=1

