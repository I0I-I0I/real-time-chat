import { Input, Modal, Button, Link, Typography } from "@/components/UI"

import styles from "./Login.module.css"

interface LoginProps {
	currentState: string
	onLinkClick: (event: React.MouseEvent<HTMLAnchorElement, MouseEvent>) => void
}

export const Login = ({currentState, onLinkClick}: LoginProps): JSX.Element => (
	<>
		<Modal className={styles.modal_login}>
			<Typography tag="h1" variant="title-1">
				{ currentState === "sing-in" ? "Sing in" : "Sing Up" }
			</Typography>
			<form action="POST" className="form sing-in">
				<Input className="input login__input" type="text" placeholder="Login..." />
				{
					currentState === "sing-up" &&
					<Input className="input login__input" type="text" placeholder="Username..." />
				}
				<Input className="input login__input" type="password" placeholder="Password..." />
				<Button className={styles.button} type="submit">
					{ currentState === "sing-in" ? "Sing in" : "Sing Up" }
				</Button>
				<Link href="#" onClick={onLinkClick} className={styles.link}>
					{ currentState === "sing-in" ? "or sing up" : "or sing in" }
				</Link>
			</form>
		</Modal>
	</>
)
