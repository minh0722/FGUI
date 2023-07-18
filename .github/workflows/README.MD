## [@semantic-release](https://github.com/semantic-release/semantic-release)

### Overview
**semantic-release** is an action that automates the release workflow: determining the next version number, generating the release notes, and npm package publishing (which we won't use for this case). 

To do that it executes certain release steps that are implemented in each of these plugins. Some of them are not required for semantic-release to work, but they are enabled by default if not specified explicitly in **package.json** configuration file
- [@semantic-release/commit-analyzer](https://github.com/semantic-release/commit-analyzer)
- [@semantic-release/release-notes-generator](https://github.com/semantic-release/release-notes-generator)
- [@semantic-release/npm](https://github.com/semantic-release/npm)
- [@semantic-release/github](https://github.com/semantic-release/github)

Additional official and community plugins are listed [here](https://github.com/semantic-release/semantic-release/blob/master/docs/extending/plugins-list.md) that can be useful.

More information about plugins can be checked [here](https://github.com/semantic-release/semantic-release/blob/master/docs/usage/plugins.md)

### Configuring semantic-release

1. Download and install [NodeJs](https://nodejs.org/en/download)

2. Navigate to the project folder, and run

		npm install --save-dev semantic-release

    This will generate files `package.json` and `package_lock.json`, which `package.json` contain the configurations for semantic-release when it's run. These 2 files need to be committed in your repository.
    
3. In the file `package.json`, add following values so it looks like this

```json
{
  "devDependencies": {
    "semantic-release": "^21.0.7"
  },
  "name": "@minh0722/fgui",
  "private" : true,
  "release": {
    "branches" : ["master", "[0-9]+.[0-9x]+", "[0-9]+.[0-9x]+.[0-9x]"]
  }
}
```
- **name** - must begin with '@' (I think?), no uppercase letters and must be one word. As specified [here](https://docs.npmjs.com/creating-a-package-json-file#required-name-and-version-fields)
- **private** - so that it won't publish npm package. This requires paid plan to have private packages deployed
- **release** - this will apply to branches `master`, `1.x, 2.x,...` and `1.0.x, 2.1.x,...` (we might not need this)

3. In your repository's **Settings -> Branches -> Branch protection rules**, add 3 rules with branch pattern name for `master`, `*.*` and `*.*.*`. They will apply for these types of branches:

    - `master` - the main latest development branch
    - `1.x, 2.x, ...` - release branches
    - `1.0.x, 2.1.x` - patch fix branches (we might not need this type of branch)

4. The rule should have following checkbox checked
	- **Require a pull request before merging**
	- **Require status checks to pass before merging**
		- **Require branches to be up to date before merging** - the status check name is the name of the Jobs in the check PR workflow file. This check might not show up in the search, because it needs to run at least once before.
    - **Do not allow bypassing the above settings**

4. In the workflow file that runs post merge of the Pull request

    - To setup NodeJs add following:

        ```yml
        steps:
          - uses: actions/checkout@v3
            with:
              submodules: recursive

          # setup nodejs
          - name: Setup Node.js
            uses: actions/setup-node@v3
            with:
              node-version: "18.x"
          - name: Install dependencies
            run: npm clean-install
        ```

	- And to run semantic-release itself, add following:

		```yml
        - name: Semantic Release
          env:
            GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
          run: npx semantic-release
        ```