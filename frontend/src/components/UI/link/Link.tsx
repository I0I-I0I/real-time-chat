import cls from "@/utils/cls"
import React from "react"
import styles from "./Link.module.css"

type LinkVariants = "default"

interface LinkProps extends React.AnchorHTMLAttributes<HTMLAnchorElement> {
	href: string
	children: React.ReactNode
	className?: string
	variant?: LinkVariants
}

export const Link = ({
	children,
	className = "",
	variant = "default",
	...props
}: LinkProps) => (
	<a className={cls(styles.link, styles[variant], className)} {...props}>
		{children}
	</a>
)
