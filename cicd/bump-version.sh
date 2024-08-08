v=$(git tag | cut -c2- )
echo Tag vesion is $v

jq --arg new_version "$v" '.version = $new_version' library.json > tmp.$$.json && mv tmp.$$.json library.json


