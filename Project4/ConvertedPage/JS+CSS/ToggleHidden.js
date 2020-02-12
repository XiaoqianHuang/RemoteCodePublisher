var HiddenComment=false;
var HiddenFunction=false;
var HiddenClass=false;

function ToggleHidden(classname){
    var HiddenVar=false;
    var itemlist = document.getElementsByClassName(classname);
    if(classname == 'Comment'){
        HiddenVar = HiddenComment;
        var button = document.getElementById('CommentButton');
        if(HiddenComment == false){
            HiddenComment = true;
            button.textContent = "Show Comment";
        }else{
            HiddenComment = false;
            button.textContent = "Hide Comment";
        }
    }else if(classname == 'Function'){
        HiddenVar = HiddenFunction;
        var button = document.getElementById('FunctionButton');
        if(HiddenFunction == false){
            HiddenFunction = true;
            button.textContent = "Show Function";
        }else{
            HiddenFunction = false;
            button.textContent = "Hide Function";
        }
    }else if(classname == 'Class'){
        HiddenVar = HiddenClass;
        var button = document.getElementById('ClassButton');
        if(HiddenClass == false){
            HiddenClass = true;
            button.textContent = "Show Class";
        }else{
            HiddenClass = false;
            button.textContent = "Hide Class";
        }
    }
    if(HiddenVar == false){
        for(var i = 0; i < itemlist.length; i++)
            itemlist[i].style.display = "none";
    }else{
        for(var i = 0; i < itemlist.length; i++)
            itemlist[i].style.display = "inline";
    }
}