v=$(git tag |tail -1| cut -c2- )
echo Tag vesion is v$v

jq --arg new_version "$v" '.version = $new_version' library.json > tmp.$$.json && mv tmp.$$.json library.json

# Display version key of library.json

echo Library version $(jq .version library.json)