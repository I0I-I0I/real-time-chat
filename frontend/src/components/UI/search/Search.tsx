import { Button, Input, Label } from "@/components/UI"

import styles from "./Search.module.css"

export const Search = (): JSX.Element => (
	<search className={styles.search}>
		<form action="">
			<Label htmlFor="search" variant="search">
				<Input id="search" type="search" variant="search" placeholder="Find a friend..." />
			</Label>
			<Button className={styles.button} variant="search">Go</Button>
		</form>
	</search>
)
